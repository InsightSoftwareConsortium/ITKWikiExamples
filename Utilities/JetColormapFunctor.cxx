#include "itkJetColormapFunction.h"
#include "itkRGBPixel.h"

int main( int, char *[])
{
  using PixelType = itk::RGBPixel<unsigned char>;
  using ColorMapType = itk::Function::JetColormapFunction<float, PixelType>;
  ColorMapType::Pointer colormap = ColorMapType::New();
  
  colormap->SetMinimumInputValue(0.0);
  colormap->SetMaximumInputValue(1.0);
  std::cout << "0: " << colormap->operator()(0.0f) << std::endl;
  std::cout << "0.5: " << colormap->operator()(0.5f) << std::endl;
  std::cout << "1: " << colormap->operator()(1.0f) << std::endl;
  return EXIT_SUCCESS;
}
