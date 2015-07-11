#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkLabelContourImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

namespace
{
  typedef unsigned char                 PixelType;
  typedef itk::RGBPixel<unsigned char>  RGBPixelType;
  typedef itk::Image<PixelType, 2>      ImageType;
  typedef itk::Image<RGBPixelType, 2>   RGBImageType;
}

static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  // Create or read an image
  ImageType::Pointer image;
  if( argc < 2 )
    {
    image = ImageType::New();
    CreateImage(image.GetPointer());
    }
  else
    {
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader =
      ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    image = reader->GetOutput();
    }

  // Generate connected components
  typedef itk::ConnectedComponentImageFilter <ImageType, ImageType >
    ConnectedComponentImageFilterType;
   ConnectedComponentImageFilterType::Pointer connectedComponentImageFilter
    = ConnectedComponentImageFilterType::New ();
  connectedComponentImageFilter->SetInput(image);

  // Generate contours for each component
  typedef itk::LabelContourImageFilter<ImageType, ImageType> LabelContourImageFilterType;
  LabelContourImageFilterType::Pointer labelContourImageFilter =
    LabelContourImageFilterType::New();
  labelContourImageFilter->SetInput(connectedComponentImageFilter->GetOutput());

  typedef itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbFilter = RGBFilterType::New();
  rgbFilter->SetInput( labelContourImageFilter->GetOutput() );
  rgbFilter->SetColormap( RGBFilterType::Jet );

  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");

  std::stringstream desc;
  desc << "LabelContourImageFilter";
  viewer.AddRGBImage(
    rgbFilter->GetOutput(),
    true,
    desc.str());  

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
    for(unsigned int c = 30; c < 100; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 255);
      }
  }

  // Make another square
  for(unsigned int r = 100; r < 130; r++)
    {
    for(unsigned int c = 115; c < 160; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 255);
      }
  }
}
