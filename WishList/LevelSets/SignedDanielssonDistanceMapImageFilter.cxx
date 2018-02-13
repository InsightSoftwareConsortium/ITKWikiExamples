#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkBinomialBlurImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSignedDanielssonDistanceMapImageFilter.h"
#include "itkContourExtractor2DImageFilter.h"

using UnsignedCharImageType = itk::Image<unsigned char, 2>;
using FloatImageType = itk::Image<float, 2>;
 
static void CreateImage(UnsignedCharImageType::Pointer image);

int main(int argc, char * argv[])
{
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  CreateImage(image);
  
  using SignedDanielssonDistanceMapImageFilterType = itk::SignedDanielssonDistanceMapImageFilter< UnsignedCharImageType, FloatImageType  >;
  SignedDanielssonDistanceMapImageFilterType::Pointer signedDanielssonDistanceMapImageFilter = SignedDanielssonDistanceMapImageFilterType::New();
  signedDanielssonDistanceMapImageFilter->SetInput(image);
  signedDanielssonDistanceMapImageFilter->Update();

  {
  using RescaleFilterType = itk::RescaleIntensityImageFilter< FloatImageType, UnsignedCharImageType >;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(signedDanielssonDistanceMapImageFilter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  using WriterType = itk::ImageFileWriter< UnsignedCharImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("output.png");
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();
  }
  
  using ContourExtractor2DImageFilterType = itk::ContourExtractor2DImageFilter <FloatImageType>;
  ContourExtractor2DImageFilterType::Pointer contourExtractor2DImageFilter = ContourExtractor2DImageFilterType::New();
  contourExtractor2DImageFilter->SetInput(signedDanielssonDistanceMapImageFilter->GetOutput());
  contourExtractor2DImageFilter->SetContourValue(0);
  contourExtractor2DImageFilter->Update();
  
  for(unsigned int contourId = 0; contourId < contourExtractor2DImageFilter->GetNumberOfOutputs(); ++contourId)
    {
    std::cout << "Contour " << contourId << " has " << contourExtractor2DImageFilter->GetOutput(contourId)->GetVertexList()->Size() << " points." << std::endl;
    }
  
  return EXIT_SUCCESS;
}


void CreateImage(UnsignedCharImageType::Pointer image)
{
  // Create an image
  itk::Index<2> start;
  start.Fill(0);
 
  itk::Size<2> size;
  size.Fill(100);
 
  itk::ImageRegion<2> region(start, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
 
  /*
  // Create a line of white pixels
  for(unsigned int i = 40; i < 60; ++i)
    {
    itk::Index<2> pixel;
    pixel.Fill(i);
    image->SetPixel(pixel, 255);
    }
  */
  for(unsigned int i = 40; i < 60; ++i)
    {
    itk::Index<2> pixel;
    pixel[0] = 40;
    pixel[1] = i;
    image->SetPixel(pixel, 255);
  
    pixel[0] = i;
    pixel[1] = 40;
    image->SetPixel(pixel, 255);
  
    pixel[0] = i;
    pixel[1] = 60;
    image->SetPixel(pixel, 255);
    
    pixel[1] = i;
    pixel[0] = 60;
    image->SetPixel(pixel, 255);
    }

  using WriterType = itk::ImageFileWriter< UnsignedCharImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("input.png");
  writer->SetInput(image);
  writer->Update();
}
