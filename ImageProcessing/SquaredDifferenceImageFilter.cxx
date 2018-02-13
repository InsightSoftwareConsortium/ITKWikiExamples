#include "itkImage.h"
#include "itkSquaredDifferenceImageFilter.h"
#include "itkImageRegionIterator.h"

using UnsignedCharImageType = itk::Image<unsigned char, 2>;
using FloatImageType = itk::Image<float, 2>;

static void CreateImage1(UnsignedCharImageType::Pointer image);
static void CreateImage2(UnsignedCharImageType::Pointer image);

int main(int, char *[])
{
  UnsignedCharImageType::Pointer image1 = UnsignedCharImageType::New();
  CreateImage1(image1);

  UnsignedCharImageType::Pointer image2 = UnsignedCharImageType::New();
  CreateImage2(image2);

  using SquaredDifferenceImageFilterType = itk::SquaredDifferenceImageFilter <UnsignedCharImageType, UnsignedCharImageType,
                                             FloatImageType>;

  SquaredDifferenceImageFilterType::Pointer squaredDifferenceFilter
          = SquaredDifferenceImageFilterType::New ();
  squaredDifferenceFilter->SetInput1(image1);
  squaredDifferenceFilter->SetInput2(image2);
  squaredDifferenceFilter->Update();

  return EXIT_SUCCESS;
}

void CreateImage1(UnsignedCharImageType::Pointer image)
{
  UnsignedCharImageType::IndexType start;
  start.Fill(0);

  UnsignedCharImageType::SizeType size;
  size.Fill(10);

  UnsignedCharImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();
  
  itk::ImageRegionIterator<UnsignedCharImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(255);

    ++imageIterator;
    }
}


void CreateImage2(UnsignedCharImageType::Pointer image)
{
  // Create an image with 2 connected components
  UnsignedCharImageType::IndexType start;
  start.Fill(0);

  UnsignedCharImageType::SizeType size;
  size.Fill(10);

  UnsignedCharImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<UnsignedCharImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(100);

    ++imageIterator;
    }

}
