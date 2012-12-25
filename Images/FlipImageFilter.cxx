#include "itkFixedArray.h"
#include "itkFlipImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRGBPixel.h"

#include "QuickView.h"

typedef itk::RGBPixel<unsigned char>  PixelType;
typedef itk::Image<PixelType, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  std::stringstream desc;

  itk::FixedArray<bool, 2> flipAxes;
  flipAxes[0] = false;
  flipAxes[1] = false;
  if (argc > 1)
    {
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( argv[1] );
    reader->Update();
    image = reader->GetOutput();
    desc << itksys::SystemTools::GetFilenameName(argv[1]);
    if (argc > 2)
      {
      flipAxes[atoi(argv[2])] = true;
      }
    if (argc > 3)
      {
      flipAxes[atoi(argv[3])] = true;
      }
    }
  else
    {
    CreateImage(image);
    desc << "Synthetic image";
    flipAxes[1] = true;
    }

  typedef itk::FlipImageFilter <ImageType>
          FlipImageFilterType;

  FlipImageFilterType::Pointer flipFilter
          = FlipImageFilterType::New ();
  flipFilter->SetInput(image);
  flipFilter->SetFlipAxes(flipAxes);

  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Flip, flipAxes= " << flipAxes;
  viewer.AddImage(
    flipFilter->GetOutput(),
    true,
    desc2.str());  

  viewer.ShareCameraOff();
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
