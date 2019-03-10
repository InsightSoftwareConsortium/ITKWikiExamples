#include "itkCustomColormapFunction.h"
#include "itkImage.h"
#include "itkImageRegionConstIterator.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkRGBPixel.h"

using RGBPixelType = itk::RGBPixel<unsigned char>;
using RGBImageType = itk::Image<RGBPixelType, 2>;

using FloatImageType = itk::Image<float, 2>;
using UnsignedCharImageType = itk::Image<unsigned char, 2>;

int main( int itkNotUsed( argc ), char * itkNotUsed( argv )[] )
{
  FloatImageType::Pointer image = FloatImageType::New();

  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size = {{100,20}};

  itk::ImageRegion<2> region(start, size);

  image->SetRegions(region);
  image->Allocate();

  for(unsigned int i = 0; i < size[0]; i++)
    {
    for(unsigned int j = 0; j < size[1]; j++)
      {
      itk::Index<2> pixel;
      pixel[0] = i;
      pixel[1] = j;
      image->SetPixel(pixel, i);
      }
    }

  // Custom colormap
  //using ColormapType = itk::Function::CustomColormapFunction<UnsignedCharImageType::PixelType, RGBImageType::PixelType>;
  using ColormapType = itk::Function::CustomColormapFunction<FloatImageType::PixelType, RGBImageType::PixelType>;
  ColormapType::ChannelType redChannel;
  ColormapType::ChannelType greenChannel;
  ColormapType::ChannelType blueChannel;
  
  redChannel.push_back(static_cast<ColormapType::RealType>(0.231373));
  greenChannel.push_back(static_cast<ColormapType::RealType>(0.298039));
  blueChannel.push_back(static_cast<ColormapType::RealType>(0.752941));

  redChannel.push_back(static_cast<ColormapType::RealType>(1));
  greenChannel.push_back(static_cast<ColormapType::RealType>(1));
  blueChannel.push_back(static_cast<ColormapType::RealType>(1));
  
  redChannel.push_back(static_cast<ColormapType::RealType>(0.705882));
  greenChannel.push_back(static_cast<ColormapType::RealType>(0.0156863));
  blueChannel.push_back(static_cast<ColormapType::RealType>(0.14902));
  
  ColormapType::Pointer colormap = ColormapType::New();
  colormap->SetRedChannel(redChannel);
  colormap->SetGreenChannel(greenChannel);
  colormap->SetBlueChannel(blueChannel);

  // Setup conversion
  using RGBFilterType = itk::ScalarToRGBColormapImageFilter<FloatImageType, RGBImageType>;
  RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
  rgbfilter->SetInput(image);
  
  rgbfilter->SetColormap(colormap);
  
  // Built-in color map
//   rgbfilter->SetColormap( RGBFilterType::Hot );

  {
  using WriterType = itk::ImageFileWriter< RGBImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("colormap.png");
  writer->SetInput(rgbfilter->GetOutput());
  writer->Update();
  }

  return EXIT_SUCCESS;
}
