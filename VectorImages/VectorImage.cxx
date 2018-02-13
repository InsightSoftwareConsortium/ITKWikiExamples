#include "itkVectorImage.h"

int main(int, char *[])
{
  // Create an image
  using ImageType = itk::VectorImage<float, 2>;

  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(2);

  ImageType::RegionType region(start,size);

  ImageType::Pointer image = ImageType::New();
  image->SetRegions(region);
  image->SetVectorLength(2);
  image->Allocate();

  ImageType::IndexType pixelIndex;
  pixelIndex[0] = 1;
  pixelIndex[1] = 1;

  ImageType::PixelType pixelValue = image->GetPixel(pixelIndex);

  std::cout << "pixel (1,1) = " << pixelValue << std::endl;

  using VariableVectorType = itk::VariableLengthVector<double>;
  VariableVectorType variableLengthVector;
  variableLengthVector.SetSize(2);
  variableLengthVector[0] = 1.1;
  variableLengthVector[1] = 2.2;

  image->SetPixel(pixelIndex, variableLengthVector);

  std::cout << "pixel (1,1) = " << pixelValue << std::endl;
  
  return EXIT_SUCCESS;
}
