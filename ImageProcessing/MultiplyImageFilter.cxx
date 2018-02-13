#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkMultiplyImageFilter.h"

#include "QuickView.h"

using ImageType = itk::Image<unsigned char, 2>;

static void CreateImage1(ImageType::Pointer image);
static void CreateImage2(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image1 = ImageType::New();
  CreateImage1(image1);

  ImageType::Pointer image2 = ImageType::New();
  CreateImage2(image2);

  using MultiplyImageFilterType = itk::MultiplyImageFilter <ImageType, ImageType >;

  MultiplyImageFilterType::Pointer multiplyFilter
    = MultiplyImageFilterType::New ();
  multiplyFilter->SetInput1(image1);
  multiplyFilter->SetInput2(image2);

  QuickView viewer;
  viewer.AddImage<ImageType>(image1);
  viewer.AddImage<ImageType>(image2);
  viewer.AddImage<ImageType>(multiplyFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateImage1(ImageType::Pointer image)
{
  // Create an image with 2 connected components
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

  // Make a square
  for(unsigned int r = 20; r < 80; r++)
    {
    for(unsigned int c = 20; c < 80; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 15);
      }
    }
}


void CreateImage2(ImageType::Pointer image)
{
  // Create an image with 2 connected components
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

  // Make another square
  for(unsigned int r = 40; r < 100; r++)
    {
    for(unsigned int c = 40; c < 100; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 15);
      }
    }
}
