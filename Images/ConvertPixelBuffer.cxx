#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "itkImage.h"
#include "itkDefaultConvertPixelTraits.h"
#include "itkConvertPixelBuffer.h"
#include "itkImageRandomConstIteratorWithIndex.h"

int main(int, char*[])
{
  const int xDimension = 200;
  const int yDimension = 100;

  using ComponentType = unsigned char;
  using RGBPixelType = itk::RGBPixel<ComponentType>;
  using RGBAPixelType = itk::RGBAPixel<ComponentType>;
  using RGBImageType = itk::Image< RGBPixelType, 2 >;
  using RGBAImageType = itk::Image< RGBAPixelType, 2 >;
  using TraitsType = itk::DefaultConvertPixelTraits< RGBPixelType >;
  using RGBAConverterType = itk::ConvertPixelBuffer< ComponentType, RGBPixelType, TraitsType >;

  RGBImageType::Pointer rgbImg   = RGBImageType::New();
  RGBAImageType::Pointer rgbaImg = RGBAImageType::New();

  // Create the two images
  // RGBAImage
  RGBAImageType::IndexType rgbaStart;
  rgbaStart[0] = 0;
  rgbaStart[1] = 0;

  RGBAImageType::SizeType rgbaSize;
  rgbaSize[0] = xDimension;
  rgbaSize[1] = yDimension;

  itk::ImageRegion<2> rgbaRegion(rgbaStart,rgbaSize);
  rgbaImg->SetRegions(rgbaRegion);
  rgbaImg->Allocate();

  RGBAPixelType rgbaDefault;
  rgbaDefault[0] = 127;
  rgbaDefault[1] = 100;
  rgbaDefault[2] = 230;
  rgbaDefault[3] = 255;
  rgbaImg->FillBuffer(rgbaDefault);

  // RGBImage
  itk::ImageRegion<2>     rgbRegion = rgbaRegion;
  rgbImg->SetRegions(rgbRegion);
  rgbImg->Allocate();

  size_t numberOfPixels =
    rgbImg->GetLargestPossibleRegion().GetNumberOfPixels();

  // Convert a raw buffer to a buffer of pixel types
  RGBAConverterType::Convert(
    rgbaImg->GetBufferPointer()->GetDataPointer(),
    rgbaImg->GetNumberOfComponentsPerPixel(),
    rgbImg->GetBufferPointer(),
    numberOfPixels);

  // Check a few random values
  itk::ImageRandomConstIteratorWithIndex<RGBImageType>
    rgbIterator(rgbImg, rgbImg->GetLargestPossibleRegion());
  rgbIterator.SetNumberOfSamples(numberOfPixels / 10);
  rgbIterator.GoToBegin();
  
  while(!rgbIterator.IsAtEnd())
    {
    if (rgbImg->GetPixel(rgbIterator.GetIndex())[0] !=
        rgbaImg->GetPixel(rgbIterator.GetIndex())[0] ||

        rgbImg->GetPixel(rgbIterator.GetIndex())[1] !=
        rgbaImg->GetPixel(rgbIterator.GetIndex())[1] ||

        rgbImg->GetPixel(rgbIterator.GetIndex())[2] !=
        rgbaImg->GetPixel(rgbIterator.GetIndex())[2])
      {
      std::cout << "Copy failed for index " << rgbIterator.GetIndex()
                << " got " << rgbImg->GetPixel(rgbIterator.GetIndex())
                << " but expected " << rgbaImg->GetPixel(rgbIterator.GetIndex())
                << std::endl;
      }
    ++rgbIterator;
    }
  return EXIT_SUCCESS;
}
