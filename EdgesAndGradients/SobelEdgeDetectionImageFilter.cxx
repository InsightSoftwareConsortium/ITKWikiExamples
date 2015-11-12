#include "itkImage.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "itkImageFileReader.h"

#include "itksys/SystemTools.hxx"

#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;
typedef itk::Image<float, 2>          FloatImageType;

typedef FloatImageType ImageType;
static void CreateImage(ImageType::Pointer);

int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  if (argc < 2)
    {
    CreateImage(image);
    }
  else
    {
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName (argv[1]);
    reader->Update();
    image = reader->GetOutput();
    }

  typedef itk::SobelEdgeDetectionImageFilter <ImageType, FloatImageType>
          SobelEdgeDetectionImageFilterType;
  SobelEdgeDetectionImageFilterType::Pointer sobelFilter
          = SobelEdgeDetectionImageFilterType::New();
  sobelFilter->SetInput(image);

  QuickView viewer;
  if (argc > 1)
    {
    viewer.AddImage<ImageType>(
      image,
      true,
      itksys::SystemTools::GetFilenameName(argv[1]));  
    }
  else
    {
    viewer.AddImage<ImageType>(image);
    }
  viewer.AddImage<FloatImageType>(
    sobelFilter->GetOutput(),
    true,
    "SobelEdgeDetectionImageFilter");

  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size;
  size.Fill(100);

  itk::ImageRegion<2> region(start, size);
  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(unsigned int r = 20; r < 80; r++)
    {
    for(unsigned int c = 20; c < 80; c++)
      {
      itk::Index<2> pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 15);
      }
    }
}
