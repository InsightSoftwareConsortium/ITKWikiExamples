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

int main( int argc, char *argv[])
{
  FloatImageType::Pointer image = FloatImageType::New();

  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size;
  size.Fill(20);

  itk::ImageRegion<2> region(start, size);

  image->SetRegions(region);
  image->Allocate();

  for(unsigned int i = 0; i < 20; i++)
    {
    for(unsigned int j = 0; j < 20; j++)
      {
      itk::Index<2> pixel;
      pixel[0] = i;
      pixel[1] = j;
      image->SetPixel(pixel, j);
      }
    }

  using RescaleFilterType = itk::RescaleIntensityImageFilter< FloatImageType, UnsignedCharImageType >;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(image);
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  {
  using WriterType = itk::ImageFileWriter< UnsignedCharImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("original.png");
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();
  }

  using RGBFilterType = itk::ScalarToRGBColormapImageFilter<FloatImageType, RGBImageType>;
  RGBFilterType::Pointer rgbfilter = RGBFilterType::New();
  rgbfilter->SetInput(image);
  rgbfilter->SetColormap( RGBFilterType::Hot );

  {
  using WriterType = itk::ImageFileWriter< RGBImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("hot.png");
  writer->SetInput(rgbfilter->GetOutput());
  writer->Update();
  }

  return EXIT_SUCCESS;
}
