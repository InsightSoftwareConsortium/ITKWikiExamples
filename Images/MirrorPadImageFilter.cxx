#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkMirrorPadImageFilter.h"

using ImageType = itk::Image<unsigned char, 2>;

static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  using MirrorPadImageFilterType = itk::MirrorPadImageFilter <ImageType, ImageType>;

  ImageType::SizeType lowerBound;
  lowerBound[0] = 20;
  lowerBound[1] = 30;

  ImageType::SizeType upperBound;
  upperBound[0] = 50;
  upperBound[1] = 40;

  MirrorPadImageFilterType::Pointer padFilter
          = MirrorPadImageFilterType::New();
  padFilter->SetInput(image);
  padFilter->SetPadLowerBound(lowerBound);
  padFilter->SetPadUpperBound(upperBound);
  padFilter->Update();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create an image
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = 200;
  size[1] = 100;

  itk::ImageRegion<2> region(start,size);

  image->SetRegions(region);
  image->Allocate();
  itk::ImageRegionIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    double distance = sqrt(pow(100.0 - imageIterator.GetIndex()[0], 2) + pow(50.0 - imageIterator.GetIndex()[1],2));
    imageIterator.Set(static_cast<unsigned char>(distance));
    ++imageIterator;
    }
}
