#include "itkImage.h"
#include "itkOtsuThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

namespace
{
typedef unsigned char             PixelType;
typedef itk::Image<PixelType, 2>  ImageType;
}

static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image;
  if( argc < 2 )
    {
    image = ImageType::New();
    CreateImage(image.GetPointer());
    }
  else
    {
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader =
      ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    image = reader->GetOutput();
    }

  typedef itk::OtsuThresholdImageFilter <ImageType, ImageType>
          FilterType;
  FilterType::Pointer otsuFilter
          = FilterType::New();
  otsuFilter->SetInput(image);
  otsuFilter->Update(); // To compute threshold

  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");

  std::stringstream desc;
  desc << "Otsu Threshold: "
       << itk::NumericTraits<FilterType::InputPixelType>::PrintType(otsuFilter->GetThreshold());
  viewer.AddImage(
    otsuFilter->GetOutput(),
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
