#include "itkVectorImage.h"
#include "itkVectorCastImageFilter.h"

int main(int argc, char *argv[])
{
  typedef itk::VectorImage<unsigned char, 2>  UnsignedCharVectorImageType;
  typedef itk::VectorImage<float, 2>  FloatVectorImageType;

  FloatVectorImageType::Pointer image = FloatVectorImageType::New();

  typedef itk::VectorCastImageFilter< FloatVectorImageType, UnsignedCharVectorImageType > VectorCastImageFilterType;
  VectorCastImageFilterType::Pointer vectorCastImageFilter = VectorCastImageFilterType::New();
  vectorCastImageFilter->SetInput(image);
  vectorCastImageFilter->Update();

  return EXIT_SUCCESS;
}
