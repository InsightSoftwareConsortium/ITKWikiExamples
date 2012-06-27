#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkFFTNormalizedCorrelationImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkImageKernelOperator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkMinimumMaximumImageCalculator.h"

#include <iostream>
#include <string>

typedef itk::Image<unsigned char, 2> ImageType;
typedef itk::Image<float, 2> FloatImageType;
typedef itk::Image<unsigned char, 2> MaskType;

void CreateMask(MaskType* const mask);

void CreateImage(ImageType::Pointer image, const itk::Index<2>& cornerOfSquare);

template <typename TImage>
void WriteImage(TImage* const image, const std::string& filename);

int main(int argc, char *argv[])
{
  itk::Index<2> offset;
  offset[0] = 5;
  offset[1] = 6;

  // Setup mask
  MaskType::Pointer mask = MaskType::New();
  CreateMask(mask);
  
  ImageType::Pointer fixedImage = ImageType::New();
  itk::Index<2> cornerOfFixedSquare;
  cornerOfFixedSquare[0] = 3;
  cornerOfFixedSquare[1] = 8;
  CreateImage(fixedImage, cornerOfFixedSquare);
  WriteImage(fixedImage.GetPointer(), "fixedImage.png");

  ImageType::Pointer movingImage = ImageType::New();
  itk::Index<2> cornerOfMovingSquare;
  cornerOfMovingSquare[0] = cornerOfFixedSquare[0] + offset[0];
  cornerOfMovingSquare[1] = cornerOfFixedSquare[1] + offset[1];
  CreateImage(movingImage, cornerOfMovingSquare);
  WriteImage(movingImage.GetPointer(), "movingImage.png");

  // Perform normalized correlation
  typedef itk::FFTNormalizedCorrelationImageFilter<ImageType, FloatImageType> CorrelationFilterType;
  CorrelationFilterType::Pointer correlationFilter = CorrelationFilterType::New();
  correlationFilter->SetFixedImage(fixedImage);
  correlationFilter->SetMovingImage(movingImage);
  correlationFilter->SetMovingImageMask(mask);
  //correlationFilter->SetFixedImageMask(mask);
  correlationFilter->Update();

  WriteImage(correlationFilter->GetOutput(), "correlation.mha");

  typedef itk::RescaleIntensityImageFilter<FloatImageType, ImageType> RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(correlationFilter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();
  WriteImage(rescaleFilter->GetOutput(), "correlation.png");

  typedef itk::MinimumMaximumImageCalculator<FloatImageType> MinimumMaximumImageCalculatorType;
  MinimumMaximumImageCalculatorType::Pointer minimumMaximumImageCalculatorFilter = MinimumMaximumImageCalculatorType::New ();
  minimumMaximumImageCalculatorFilter->SetImage(correlationFilter->GetOutput());
  minimumMaximumImageCalculatorFilter->Compute();

  itk::Index<2> maximumCorrelationPatchCenter = minimumMaximumImageCalculatorFilter->GetIndexOfMaximum();

  itk::Size<2> outputSize = correlationFilter->GetOutput()->GetLargestPossibleRegion().GetSize();

  itk::Index<2> maximumCorrelationPatchCenterFixed;
  maximumCorrelationPatchCenterFixed[0] = outputSize[0]/2 - maximumCorrelationPatchCenter[0];
  maximumCorrelationPatchCenterFixed[1] = outputSize[1]/2 - maximumCorrelationPatchCenter[1];

  std::cout << "Maximum location: " << maximumCorrelationPatchCenter << std::endl;
  std::cout << "Maximum location fixed: " << maximumCorrelationPatchCenterFixed << std::endl; // This is the value we expect!
  std::cout << "Maximum value: " << minimumMaximumImageCalculatorFilter->GetMaximum() << std::endl; // If the images can be perfectly aligned, the value is 1

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image, const itk::Index<2>& cornerOfSquare)
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

template <typename TImage>
void WriteImage(TImage* const image, const std::string& filename)
{
  typedef  itk::ImageFileWriter<TImage> WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(filename);
  writer->SetInput(image);
  writer->Update();
}


void CreateMask(MaskType* const mask)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(51);

  ImageType::RegionType region(start,size);

  mask->SetRegions(region);
  mask->Allocate();
  mask->FillBuffer(255); // Make the whole mask "valid"

  //unsigned int squareSize = 8;
  unsigned int squareSize = 3;

  itk::Index<2> cornerOfSquare = {{3,8}};

  // Remove pixels from the mask in a small square. The correlationw will not be computed at these pixels.
  itk::ImageRegionIterator<MaskType> maskIterator(mask, region);

  while(!maskIterator.IsAtEnd())
    {
    if(maskIterator.GetIndex()[0] > cornerOfSquare[0] &&
       maskIterator.GetIndex()[0] < cornerOfSquare[0] + squareSize &&
       maskIterator.GetIndex()[1] > cornerOfSquare[1] &&
       maskIterator.GetIndex()[1] < cornerOfSquare[1] + squareSize)
      {
      maskIterator.Set(0);
      }

    ++maskIterator;
    }
}
