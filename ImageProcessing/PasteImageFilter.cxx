#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkPasteImageFilter.h"

#include "itkImageToVTKImageFilter.h"

#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage1(ImageType::Pointer image);
static void CreateImage2(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image1 = ImageType::New();
  CreateImage1(image1);

  ImageType::Pointer image2 = ImageType::New();
  CreateImage2(image2);

  typedef itk::PasteImageFilter <ImageType, ImageType >
    PasteImageFilterType;

  // The SetDestinationIndex() method prescribes where in the first
  // input to start pasting data from the second input.
  // The SetSourceRegion method prescribes the section of the second
  // image to paste into the first.

  ImageType::IndexType destinationIndex;
  destinationIndex[0] = 10;
  destinationIndex[1] = 10;
  
  PasteImageFilterType::Pointer pasteFilter
    = PasteImageFilterType::New ();
  pasteFilter->SetSourceImage(image1);
  pasteFilter->SetDestinationImage(image2);
  pasteFilter->SetSourceRegion(image1->GetLargestPossibleRegion());
  pasteFilter->SetDestinationIndex(destinationIndex);

  QuickView viewer;
  viewer.AddImage(
    image1.GetPointer());
  viewer.AddImage(
    image2.GetPointer());
  viewer.AddImage(
    pasteFilter->GetOutput());

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

      image->SetPixel(pixelIndex, 255);
      }
    }
}


void CreateImage2(ImageType::Pointer image)
{
  // Create an image
  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  unsigned int NumRows = 400;
  unsigned int NumCols = 500;
  size[0] = NumRows;
  size[1] = NumCols;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  // Make another square
  for(unsigned int r = 0; r < NumRows; r++)
    {
    for(unsigned int c = 0; c < NumCols; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 200);
      }
    }
}
