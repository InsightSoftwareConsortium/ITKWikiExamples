#include "itkVectorImage.h"
#include "itkVectorCastImageFilter.h"

int main(int argc, char *argv[])
{
  using UnsignedCharVectorImageType = itk::VectorImage<unsigned char, 2>;
  using FloatVectorImageType = itk::VectorImage<float, 2>;

  FloatVectorImageType::Pointer image = FloatVectorImageType::New();

  using VectorCastImageFilterType = itk::VectorCastImageFilter< FloatVectorImageType, UnsignedCharVectorImageType >;
  VectorCastImageFilterType::Pointer vectorCastImageFilter = VectorCastImageFilterType::New();
  vectorCastImageFilter->SetInput(image);
  vectorCastImageFilter->Update();

  return EXIT_SUCCESS;
}
