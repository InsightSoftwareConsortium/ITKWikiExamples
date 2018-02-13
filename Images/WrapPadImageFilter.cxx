#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkWrapPadImageFilter.h"
#include "itkImageRegionIterator.h"

using ImageType = itk::Image<unsigned char, 2>;

static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  using WrapPadImageFilterType = itk::WrapPadImageFilter <ImageType, ImageType>;

  ImageType::SizeType lowerBound;
  lowerBound[0] = 20;
  lowerBound[1] = 30;

  ImageType::SizeType upperBound;
  upperBound[0] = 50;
  upperBound[1] = 40;
  
  WrapPadImageFilterType::Pointer padFilter
          = WrapPadImageFilterType::New();
  padFilter->SetInput(image);
  padFilter->SetPadLowerBound(lowerBound);
  padFilter->SetPadUpperBound(upperBound);
  padFilter->Update();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create an image
  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = 200;
  size[1] = 100;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();
  itk::ImageRegionIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(imageIterator.GetIndex()[0]);
    ++imageIterator;
    }
}
