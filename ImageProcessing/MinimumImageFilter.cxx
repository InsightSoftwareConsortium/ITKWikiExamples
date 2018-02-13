#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkMinimumImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIterator.h"

using ImageType = itk::Image<unsigned char, 2>;

static void CreateImage1(ImageType* image);
static void CreateImage2(ImageType* image);

int main(int, char*[])
{
  ImageType::Pointer image1 = ImageType::New();
  CreateImage1(image1);

  ImageType::Pointer image2 = ImageType::New();
  CreateImage2(image2);

  using MinimumImageFilterType = itk::MinimumImageFilter <ImageType>;

  MinimumImageFilterType::Pointer minimumImageFilter
    = MinimumImageFilterType::New ();
  minimumImageFilter->SetInput(0, image1);
  minimumImageFilter->SetInput(1, image2);
  minimumImageFilter->Update();

  return EXIT_SUCCESS;
}

void CreateImage1(ImageType* image)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(100);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] < 30)
      {
      imageIterator.Set(255);
      }
    else
      {
      imageIterator.Set(0);
      }

    ++imageIterator;
    }

}

void CreateImage2(ImageType* image)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(100);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > 70)
      {
      imageIterator.Set(255);
      }
    else
      {
      imageIterator.Set(0);
      }

    ++imageIterator;
    }

}
