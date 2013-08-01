#include "itkImage.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

typedef itk::Image<float, 2> ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  unsigned int radius;
  double alpha, beta;
  if (argc < 2)
    {
    CreateImage(image);
    }
  else
    {
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    image = reader->GetOutput();
    if (argc > 2)
      {
      radius = atoi(argv[2]);
      }
    if (argc > 3)
      {
      alpha = atoi(argv[3]);
      }
    if (argc > 4)
      {
      beta = atoi(argv[4]);
      }
    }

  typedef  itk::AdaptiveHistogramEqualizationImageFilter< ImageType > AdaptiveHistogramEqualizationImageFilterType;
  AdaptiveHistogramEqualizationImageFilterType::Pointer adaptiveHistogramEqualizationImageFilter = AdaptiveHistogramEqualizationImageFilterType::New();
  adaptiveHistogramEqualizationImageFilter->SetInput(image);
  adaptiveHistogramEqualizationImageFilter->SetRadius(radius);
  adaptiveHistogramEqualizationImageFilter->SetAlpha(alpha);
  adaptiveHistogramEqualizationImageFilter->SetBeta(beta);
 
  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");  

  std::stringstream desc;
  desc << "Adaptive Histogram Equalization\n"
       << "Radius: " << adaptiveHistogramEqualizationImageFilter->GetRadius()
       << ", Alpha: " << adaptiveHistogramEqualizationImageFilter->GetAlpha()
       << ", Beta: " << adaptiveHistogramEqualizationImageFilter->GetBeta();
  viewer.AddImage(
    adaptiveHistogramEqualizationImageFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

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
