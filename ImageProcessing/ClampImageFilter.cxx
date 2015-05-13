#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkClampImageFilter.h"

typedef itk::Image<float, 2>  FloatImageType;
typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;

static void CreateImage(FloatImageType* const image);

int main(int argc, char *argv[])
{
  FloatImageType::Pointer image;

  // No input image argument provided
  if(argc < 2)
    {
    image = FloatImageType::New();
    CreateImage(image);
    }
  else // Input image argument provided
    {
    typedef itk::ImageFileReader<FloatImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    image = reader->GetOutput();
    }

  typedef itk::ClampImageFilter< FloatImageType, UnsignedCharImageType > ClampFilterType;
  ClampFilterType::Pointer clampFilter = ClampFilterType::New();
  clampFilter->SetInput(image);
  clampFilter->Update();

  return EXIT_SUCCESS;
}

void CreateImage(FloatImageType* const image)
{
  // Create an image with 2 connected components
  FloatImageType::IndexType corner = {{0,0}};

  FloatImageType::SizeType size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  FloatImageType::RegionType region(corner, size);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(unsigned int r = 40; r < 100; r++)
    {
    for(unsigned int c = 40; c < 100; c++)
      {
      FloatImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 15);
      }
    }
}
