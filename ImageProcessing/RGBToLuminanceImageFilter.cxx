#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRGBToLuminanceImageFilter.h"

#include "QuickView.h"

typedef itk::Image<itk::RGBPixel<unsigned char>, 2> RGBImageType;
typedef itk::Image<unsigned char, 2>  ScalarImageType;

static void CreateImage(RGBImageType* const image);

int main(int argc, char *argv[])
{
  RGBImageType::Pointer image;

  if(argc < 2)
    {
    //std::cerr << "Required: filename" << std::endl;
    //return EXIT_FAILURE;
    image = RGBImageType::New();
    CreateImage(image);
    }
  else
  {
    typedef itk::ImageFileReader<RGBImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    image = reader->GetOutput();
  }

  typedef itk::RGBToLuminanceImageFilter<RGBImageType, ScalarImageType> LuminanceFilterType;
  LuminanceFilterType::Pointer luminanceFilter = LuminanceFilterType::New();
  luminanceFilter->SetInput(image);
  luminanceFilter->Update();

  QuickView viewer;
  viewer.AddRGBImage(image.GetPointer());
  //viewer.AddRGBImage(image.GetPointer());
  viewer.AddImage(luminanceFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}


void CreateImage(RGBImageType* const image)
{
  // Create an image with 2 connected components
  itk::Index<2> corner = {{0,0}};

  itk::Size<2> size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  itk::ImageRegion<2> region(corner, size);

  image->SetRegions(region);
  image->Allocate();

  RGBImageType::PixelType pixel;
  pixel.SetRed(200);
  pixel.SetGreen(50);
  pixel.SetBlue(50);
  // Make a square
  for(unsigned int r = 40; r < 100; r++)
    {
    for(unsigned int c = 40; c < 100; c++)
      {
      itk::Index<2> pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, pixel);
      }
    }
}
