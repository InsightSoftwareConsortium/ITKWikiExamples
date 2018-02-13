#include <itkImage.h>
#include <itkRGBPixel.h>

int main(int argc, char *argv[])
{
  using RGBPixelType = itk::RGBPixel<unsigned char>;
  using RGBImageType = itk::Image<RGBPixelType>;
  RGBImageType::Pointer image = RGBImageType::New();
  
  return EXIT_SUCCESS;
}
