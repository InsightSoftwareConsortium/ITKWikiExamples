#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkNormalizedCorrelationImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkImageKernelOperator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkMinimumMaximumImageCalculator.h"

#include <iostream>
#include <string>

typedef itk::Image<float, 2> ImageType;

//typedef itk::Image<float, 2> FloatImageType;
//typedef itk::Image<unsigned char, 2> UnsignedCharImageType;

void CreateImage(ImageType::Pointer image);
void CreateImageOfSquare(ImageType::Pointer image, const itk::Index<2>& cornerOfSquare);
void WriteImage(ImageType::Pointer image, const std::string& filename);
void FillRegion(ImageType::Pointer image, const itk::ImageRegion<2>& region, ImageType::PixelType value);

int main(int argc, char *argv[])
{
  // Setup mask
  ImageType::Pointer mask = ImageType::New();
  CreateImage(mask);
  
  itk::Index<2> maskIndex;
  maskIndex[0] = mask->GetLargestPossibleRegion().GetSize()[0]/2;
  maskIndex[1] = 0;
  
  itk::Size<2> maskSize;
  maskSize[0] = mask->GetLargestPossibleRegion().GetSize()[0]/2 + 1;
  maskSize[1] = mask->GetLargestPossibleRegion().GetSize()[1];
  
  itk::ImageRegion<2> maskRegion(maskIndex, maskSize);
  FillRegion(mask, maskRegion, 255);
  WriteImage(mask, "mask.mha");

  // Setup image1
  ImageType::Pointer image1 = ImageType::New();
  itk::Index<2> cornerOfSquare1;
  cornerOfSquare1[0] = 3;
  cornerOfSquare1[1] = 8;
  CreateImageOfSquare(image1, cornerOfSquare1);
  WriteImage(image1, "image1.mha");
  
  // Setup image2
  itk::Index<2> offset;
  offset[0] = 20;
  offset[1] = 6;
  
  ImageType::Pointer image2 = ImageType::New();
  itk::Index<2> cornerOfSquare2;
  cornerOfSquare2[0] = cornerOfSquare1[0] + offset[0];
  cornerOfSquare2[1] = cornerOfSquare1[1] + offset[1];
  CreateImageOfSquare(image2, cornerOfSquare2);
  FillRegion(image2, maskRegion, 122); // If this line is uncommented, the result is infinity!?
  WriteImage(image2, "image2.mha");

  // Perform normalized correlation
  // <input type, mask type, output type>
  typedef itk::NormalizedCorrelationImageFilter<ImageType, ImageType, ImageType> CorrelationFilterType;

  itk::ImageKernelOperator<float> kernelOperator;
  kernelOperator.SetImageKernel(image1);

  // The radius of the kernel must be the radius of the patch, NOT the size of the patch
  itk::Size<2> radius;
  radius[0] = image1->GetLargestPossibleRegion().GetSize()[0]/2;
  radius[1] = image1->GetLargestPossibleRegion().GetSize()[1]/2;

  if(radius[0] % 2 == 0 || radius[1] % 2 == 0)
    {
    std::cerr << "Input must have odd dimensions!" << std::endl;
    return EXIT_FAILURE;
    }

  kernelOperator.CreateToRadius(radius);

  CorrelationFilterType::Pointer correlationFilter = CorrelationFilterType::New();
  correlationFilter->SetInput(image2);
  //correlationFilter->SetMaskImage(mask);
  correlationFilter->SetTemplate(kernelOperator);
  correlationFilter->Update();

  WriteImage(correlationFilter->GetOutput(), "correlation.mha");

  typedef itk::MinimumMaximumImageCalculator <ImageType> MinimumMaximumImageCalculatorType;

  MinimumMaximumImageCalculatorType::Pointer minimumMaximumImageCalculatorFilter = MinimumMaximumImageCalculatorType::New ();
  minimumMaximumImageCalculatorFilter->SetImage(correlationFilter->GetOutput());
  minimumMaximumImageCalculatorFilter->Compute();

  itk::Index<2> maximumCorrelationPatchCenter = minimumMaximumImageCalculatorFilter->GetIndexOfMaximum();
  //std::cout << "Maximum location: " << maximumCorrelationPatchCenter << std::endl;
  std::cout << "Maximum location fixed: " << maximumCorrelationPatchCenter - radius << std::endl; // This is the value we expect!
  std::cout << "Maximum value: " << minimumMaximumImageCalculatorFilter->GetMaximum() << std::endl; // If the images can be perfectly aligned, the value is 1

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(51);

  ImageType::RegionType region(start,size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
}

void CreateImageOfSquare(ImageType::Pointer image, const itk::Index<2>& cornerOfSquare)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(51);

  ImageType::RegionType region(start,size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  unsigned int squareSize = 8;

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > cornerOfSquare[0] && imageIterator.GetIndex()[0] < cornerOfSquare[0] + squareSize &&
      imageIterator.GetIndex()[1] > cornerOfSquare[1] && imageIterator.GetIndex()[1] < cornerOfSquare[1] + squareSize)
      {
      imageIterator.Set(255);
      }

    ++imageIterator;
    }
}

void WriteImage(ImageType::Pointer image, const std::string& filename)
{
  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(filename);
  writer->SetInput(image);
  writer->Update();

}

void FillRegion(ImageType::Pointer image, const itk::ImageRegion<2>& region, ImageType::PixelType value)
{
  itk::ImageRegionIterator<ImageType> imageIterator(image, region);

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(value);

    ++imageIterator;
    }

}
