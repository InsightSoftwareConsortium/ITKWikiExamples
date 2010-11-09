#include "itkVectorImage.h"

int main(int, char *[])
{
  typedef itk::VectorImage<float, 2>  ImageType;

  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = 2;
  size[1] = 3;

  region.SetSize(	size);
  region.SetIndex(start);

  ImageType::Pointer image = ImageType::New();
  image->SetRegions(region);
  image->SetVectorLength(2);
  image->Allocate();

  ImageType::IndexType pixelIndex;
  pixelIndex[0] = 1;
  pixelIndex[1] = 1;

  ImageType::PixelType pixelValue = image->GetPixel(pixelIndex);

  std::cout << "pixel (1,1) = " << pixelValue << std::endl;

  return EXIT_SUCCESS;
}
