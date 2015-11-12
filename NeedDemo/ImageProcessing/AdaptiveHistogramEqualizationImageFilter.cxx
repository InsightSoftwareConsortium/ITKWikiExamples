#include "itkImage.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include "itkImageFileWriter.h"

typedef itk::Image<float, 2> ImageType;

void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  
  typedef  itk::AdaptiveHistogramEqualizationImageFilter< ImageType > AdaptiveHistogramEqualizationImageFilterType;
  AdaptiveHistogramEqualizationImageFilterType::Pointer adaptiveHistogramEqualizationImageFilter = AdaptiveHistogramEqualizationImageFilterType::New();
  adaptiveHistogramEqualizationImageFilter->SetInput(image);
  adaptiveHistogramEqualizationImageFilter->SetRadius(1);
  adaptiveHistogramEqualizationImageFilter->Update();
 
  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("output.mhd");
  writer->SetInput(adaptiveHistogramEqualizationImageFilter->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create an image that is mostly constant but has some different kinds of objects.
  ImageType::IndexType start;
  start.Fill(0);
 
  ImageType::SizeType size;
  size.Fill(100);
 
  ImageType::RegionType region(start,size);
 
  // Create a black image
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
 
  // Create a white square
  itk::ImageRegionIterator<ImageType> imageIterator(image,region);
 
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > 50 && imageIterator.GetIndex()[0] < 70 &&
       imageIterator.GetIndex()[1] > 50 && imageIterator.GetIndex()[1] < 70 )
      {
      imageIterator.Set(255);
      }
    ++imageIterator;
    }

  // Create a rogue white pixel
  ImageType::IndexType pixel;
  pixel.Fill(20);
  image->SetPixel(pixel, 255);

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("input.mhd");
  writer->SetInput(image);
  writer->Update();
}
