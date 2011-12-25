#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRelabelComponentImageFilter.h"

#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  
  itk::RelabelComponentImageFilter<ImageType, ImageType>::Pointer relabelFilter =
    itk::RelabelComponentImageFilter<ImageType, ImageType>::New();
  relabelFilter->SetInput(image);

  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),
    true,
    "Original");

  viewer.AddImage(
    relabelFilter->GetOutput(),
    true,
    "Relabeled");

  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create an image with 2 connected components
  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = 200;
  size[1] = 300;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > 100 &&
      imageIterator.GetIndex()[0] < 150 &&
      imageIterator.GetIndex()[1] > 100 &&
      imageIterator.GetIndex()[1] < 150)
      {
      imageIterator.Set(100);
      }
    else if(imageIterator.GetIndex()[0] > 50 &&
      imageIterator.GetIndex()[0] < 70 &&
      imageIterator.GetIndex()[1] > 50 &&
      imageIterator.GetIndex()[1] < 70)
      {
      imageIterator.Set(200);
      }
    else
      {
      imageIterator.Set(0);
      }

    ++imageIterator;
  }
}
