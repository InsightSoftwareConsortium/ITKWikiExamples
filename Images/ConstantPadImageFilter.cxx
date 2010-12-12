#include "itkImage.h"
#include "itkConstantPadImageFilter.h"
#include "itkImageRegionIterator.h"

#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  typedef itk::ConstantPadImageFilter <ImageType, ImageType>
    ConstantPadImageFilterType;

  ImageType::SizeType lowerExtendRegion;
  lowerExtendRegion[0] = 20;
  lowerExtendRegion[1] = 30;

  ImageType::SizeType upperExtendRegion;
  upperExtendRegion[0] = 50;
  upperExtendRegion[1] = 40;

  ImageType::PixelType constantPixel = 100;

  ConstantPadImageFilterType::Pointer padFilter
    = ConstantPadImageFilterType::New();
  padFilter->SetInput(image);
  padFilter->SetPadLowerBound(lowerExtendRegion);
  padFilter->SetPadUpperBound(upperExtendRegion);
  padFilter->SetConstant(constantPixel);

  QuickView viewer;
  viewer.AddImage<ImageType>(image);
  viewer.AddImage<ImageType>(padFilter->GetOutput());
  viewer.Visualize();

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
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  // Make the whole image white
  itk::ImageRegionIterator<ImageType> iterator(image,image->GetLargestPossibleRegion());

  while(!iterator.IsAtEnd())
    {
    iterator.Set(255);
    ++iterator;
    }
}
