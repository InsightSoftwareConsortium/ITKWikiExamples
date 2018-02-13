#include "itkImage.h"
#include "itkContinuousIndex.h"
#include "itkLinearInterpolateImageFunction.h"

using ImageType = itk::Image<unsigned char, 1>;

static void CreateImage(ImageType::Pointer image);

int main(int, char*[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  itk::ContinuousIndex<double, 1> pixel;
  pixel[0] = 1.3;

  itk::LinearInterpolateImageFunction<ImageType, double>::Pointer interpolator =
    itk::LinearInterpolateImageFunction<ImageType, double>::New();
  interpolator->SetInputImage(image);
  
  std::cout << "Value at 1.3: " << interpolator->EvaluateAtContinuousIndex(pixel) << std::endl;

  return EXIT_SUCCESS;
}


void CreateImage(ImageType::Pointer image)
{
  // Create a 1D image
  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;

  ImageType::SizeType size;
  size[0] = 10;
  
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  for(unsigned int i = 0; i < 10; i++)
    {
    ImageType::IndexType pixelIndex;
    pixelIndex[0] = i;

    image->SetPixel(pixelIndex, i*10);
    }

}
