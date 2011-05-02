#include "itkFixedArray.h"
#include "itkFlipImageFilter.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  typedef itk::FlipImageFilter <ImageType>
          FlipImageFilterType;

  FlipImageFilterType::Pointer flipFilter
          = FlipImageFilterType::New ();
  flipFilter->SetInput(image);

  itk::FixedArray<bool, 2> flipAxes;
  flipAxes[0] = false;
  flipAxes[1] = true;

  flipFilter->SetFlipAxes(flipAxes);
  flipFilter->Update();

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
