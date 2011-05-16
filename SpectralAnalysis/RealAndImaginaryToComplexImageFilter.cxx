#include "itkImage.h"
#include "itkRealAndImaginaryToComplexImageFilter.h"

#include <complex>

int main( int argc, char *argv[] )
{
  typedef itk::Image<unsigned char, 2> ImageType;
  typedef itk::Image<std::complex<float>, 2> ComplexImageType;

  ImageType::Pointer realImage = ImageType::New();
  ImageType::Pointer imaginaryImage = ImageType::New();
    
  typedef itk::RealAndImaginaryToComplexImageFilter<ImageType,ImageType,ComplexImageType> RealAndImaginaryToComplexImageFilterType;
  RealAndImaginaryToComplexImageFilterType::Pointer realAndImaginaryToComplexImageFilter = RealAndImaginaryToComplexImageFilterType::New();
  realAndImaginaryToComplexImageFilter->SetInput1(realImage);
  realAndImaginaryToComplexImageFilter->SetInput2(imaginaryImage);
  realAndImaginaryToComplexImageFilter->Update();

  ComplexImageType* output = realAndImaginaryToComplexImageFilter->GetOutput();
  output->Print(std::cout);

  return EXIT_SUCCESS;
}
