#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

template <typename TImage>
static void CreateImage(TImage* const image);

int main( int argc, char *argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                       PixelType;
  typedef itk::RGBPixel<unsigned char>         RGBPixelType;
  typedef itk::Image<PixelType, Dimension>     ImageType;
  typedef itk::Image<RGBPixelType, Dimension>  RGBImageType;

  ImageType::Pointer image;
  if( argc < 2 )
    {
    image = ImageType::New();
    CreateImage(image.GetPointer());
    }
  else
    {
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    image = reader->GetOutput();
    }

  typedef itk::Image< unsigned short, Dimension > OutputImageType;

  typedef itk::ConnectedComponentImageFilter <ImageType, OutputImageType >
    ConnectedComponentImageFilterType;

  ConnectedComponentImageFilterType::Pointer connected =
    ConnectedComponentImageFilterType::New ();
  connected->SetInput(image);
  connected->Update();

  std::cout << "Number of objects: " << connected->GetObjectCount() << std::endl;

  typedef itk::ScalarToRGBColormapImageFilter<OutputImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbFilter = RGBFilterType::New();
  rgbFilter->SetInput( connected->GetOutput() );
  rgbFilter->SetColormap( RGBFilterType::Jet );

  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");  

  std::stringstream desc;
  desc << "Connected Components: "
       << connected->GetObjectCount() << " objects";
  viewer.AddRGBImage(
    rgbFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}

template <typename TImage>
void CreateImage(TImage* const image)
{
  // Create an image with 2 connected components
  typename TImage::IndexType start = {{0,0}};
  start[0] = 0;
  start[1] = 0;

  typename TImage::SizeType size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  typename TImage::RegionType region(start, size);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(typename TImage::IndexValueType r = 20; r < 80; r++)
    {
    for(typename TImage::IndexValueType c = 30; c < 100; c++)
      {
      typename TImage::IndexType pixelIndex = {{r,c}};

      image->SetPixel(pixelIndex, 255);
      }
    }

  // Make another square
  for(typename TImage::IndexValueType r = 100; r < 130; r++)
    {
    for(typename TImage::IndexValueType c = 115; c < 160; c++)
      {
      typename TImage::IndexType pixelIndex = {{r,c}};

      image->SetPixel(pixelIndex, 255);
      }
    }
}
