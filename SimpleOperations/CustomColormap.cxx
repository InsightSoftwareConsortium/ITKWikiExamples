#include "itkCustomColormapFunction.h"
#include "itkScalarToRGBColormapImageFilter.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkCustomColormapFunction.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkRGBPixel.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

#include "QuickView.h"

typedef itk::RGBPixel<unsigned char> RGBPixelType;
typedef itk::Image<RGBPixelType, 2>  RGBImageType;
typedef itk::Image<unsigned char, 2> ImageType;
typedef itk::Function::CustomColormapFunction<
  ImageType::PixelType, RGBImageType::PixelType> ColormapType;

static void CreateRandomImage(
  ImageType::Pointer image, unsigned int width = 200, unsigned height = 300);
static void CreateRandomColormap(
  ColormapType::Pointer colormap, unsigned int size = 255);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();

  CreateRandomImage(image);
  
  typedef itk::ScalarToRGBColormapImageFilter<ImageType, RGBImageType> ColormapFilterType;
  ColormapFilterType::Pointer colormapFilter1 =
    ColormapFilterType::New();

  ColormapType::Pointer largeColormap
    = ColormapType::New();
  CreateRandomColormap(largeColormap, 255);

  colormapFilter1->SetInput (image);
  colormapFilter1->SetColormap(largeColormap);

  QuickView viewer;
  viewer.AddRGBImage(
    colormapFilter1->GetOutput(),
    true);

  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateRandomImage(ImageType::Pointer image,\
                       unsigned int width,
                       unsigned height)
{
  itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer random =
    itk::Statistics::MersenneTwisterRandomVariateGenerator::New();
  // Create an image with 2 connected components
  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = width;
  size[1] = height;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  unsigned int cnt = 0;

  ImageType::PixelType value = random->GetUniformVariate(0.0, 255.0);
  unsigned int bandSize = 5;
  while(!imageIterator.IsAtEnd())
    {
    if (cnt % (size[0] * bandSize) == 0)
      {
      value = random->GetUniformVariate(0.0, 255.0);
      bandSize = static_cast<unsigned int>(random->GetUniformVariate(5, 10));
      }
      imageIterator.Set(static_cast<ImageType::PixelType>(value));
    ++imageIterator;
    ++cnt;
    }
}

void CreateRandomColormap(ColormapType::Pointer colormap, unsigned int size)
{
  ColormapType::ChannelType redChannel;
  ColormapType::ChannelType greenChannel;
  ColormapType::ChannelType blueChannel;
  itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer random =
    itk::Statistics::MersenneTwisterRandomVariateGenerator::New();

  random->SetSeed ( 8775070 );
  for (unsigned int i = 0; i < size; ++i)
    {
    redChannel.push_back(static_cast<ColormapType::RealType>
                         (random->GetUniformVariate(.3, 1.0)));
    greenChannel.push_back(static_cast<ColormapType::RealType>
                           (random->GetUniformVariate(.3, 1.0)));
    blueChannel.push_back(static_cast<ColormapType::RealType>
                          (random->GetUniformVariate(.3, 1.0)));
    }
  colormap->SetRedChannel(redChannel);
  colormap->SetGreenChannel(greenChannel);
  colormap->SetBlueChannel(blueChannel);
}
