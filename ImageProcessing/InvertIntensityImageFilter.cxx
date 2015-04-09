#include "itkImage.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkImageFileReader.h"

#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  std::stringstream desc;

  CreateImage(image);

  unsigned int maximum = 255;
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
      maximum = atoi(argv[2]);
      }
    }
  else
    {
    CreateImage(image);
    desc << "Synthetic image";
    maximum = 50;
    }

  typedef itk::InvertIntensityImageFilter <ImageType>
    InvertIntensityImageFilterType;

  InvertIntensityImageFilterType::Pointer invertIntensityFilter
    = InvertIntensityImageFilterType::New();
  invertIntensityFilter->SetInput(image);
  invertIntensityFilter->SetMaximum(maximum);

  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "InvertIntensity, maximum = " << maximum;
  viewer.AddImage(
    invertIntensityFilter->GetOutput(),
    true,
    desc2.str());  

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

  // Make a square
  for(unsigned int r = 40; r < 100; r++)
    {
    for(unsigned int c = 40; c < 100; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 50);
      }
    }
}
