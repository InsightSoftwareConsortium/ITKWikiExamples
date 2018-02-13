#include "itkImage.h"
#include "itkOtsuMultipleThresholdsImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkLabelToRGBImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

using PixelType = unsigned char;
using ImageType = itk::Image<PixelType, 2>;
using RGBPixelType = itk::RGBPixel<unsigned char>;
using RGBImageType = itk::Image<RGBPixelType, 2>;


static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  unsigned int numberOfThresholds = 1;
  ImageType::Pointer image;
  if( argc < 2 )
    {
    image = ImageType::New();
    CreateImage(image.GetPointer());
    }
  else
    {
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer reader =
      ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    if (argc > 2)
      {
      numberOfThresholds = atoi(argv[2]);
      }
    image = reader->GetOutput();
    }

  using FilterType = itk::OtsuMultipleThresholdsImageFilter <ImageType, ImageType>;
  FilterType::Pointer otsuFilter
          = FilterType::New();
  otsuFilter->SetInput(image);
  otsuFilter->SetNumberOfThresholds(numberOfThresholds);
  otsuFilter->Update(); // To compute threshold

  using RGBFilterType = itk::LabelToRGBImageFilter<ImageType, RGBImageType>;
  RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
  colormapImageFilter->SetInput(otsuFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");

  std::stringstream desc;
  desc << "Otsu Threshold\n"
       << otsuFilter->GetNumberOfThresholds()
       << " thresholds: ";
  FilterType::ThresholdVectorType thresholds = otsuFilter->GetThresholds();
  for (double threshold : thresholds)
    {
    desc << itk::NumericTraits<FilterType::InputPixelType>::PrintType(threshold) << " ";
    }

  viewer.AddRGBImage(
    colormapImageFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create an image
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(100);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  // Make the whole image white
  itk::ImageRegionIterator<ImageType> iterator(image,image->GetLargestPossibleRegion());

  /*
   //Create a square
  while(!iterator.IsAtEnd())
    {
    iterator.Set(255);
    ++iterator;
    }
  */
}
