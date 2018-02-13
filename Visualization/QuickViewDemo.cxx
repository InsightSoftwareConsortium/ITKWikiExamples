#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"

#include "QuickView.h"

using ImageType = itk::Image<unsigned char, 2>;

static void CreateImage(ImageType* const image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image;

  if(argc < 2)
    {
    //std::cerr << "Required: filename" << std::endl;
    //return EXIT_FAILURE;
    image = ImageType::New();
    CreateImage(image);
    }
  else
  {
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    image = reader->GetOutput();
  }

  using RescaleFilterType = itk::RescaleIntensityImageFilter< ImageType, ImageType >;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(image);
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  QuickView viewer;
  viewer.AddImage(image.GetPointer());
  viewer.AddImage(rescaleFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType* const image)
{
  // Create an image with 2 connected components
  ImageType::IndexType corner = {{0,0}};

  ImageType::SizeType size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  ImageType::RegionType region(corner, size);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
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
