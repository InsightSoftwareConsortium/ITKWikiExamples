#include "itkImage.h"

#if ITK_VERSION_MAJOR < 4
#include "itkRealAndImaginaryToComplexImageFilter.h"
#else
#include "itkComposeImageFilter.h"
#endif

#include <complex>

int main( int itkNotUsed( argc ), char * itkNotUsed( argv )[] )
{
  using ImageType = itk::Image<unsigned char, 2>;
  using ComplexImageType = itk::Image<std::complex<float>, 2>;

  ImageType::Pointer realImage = ImageType::New();
  ImageType::Pointer imaginaryImage = ImageType::New();
    
#if ITK_VERSION_MAJOR < 4
  using RealAndImaginaryToComplexImageFilterType = itk::RealAndImaginaryToComplexImageFilter<ImageType,ComplexImageType>;
#else
  using RealAndImaginaryToComplexImageFilterType = itk::ComposeImageFilter<ImageType,ComplexImageType>;
#endif
  RealAndImaginaryToComplexImageFilterType::Pointer realAndImaginaryToComplexImageFilter = RealAndImaginaryToComplexImageFilterType::New();
  realAndImaginaryToComplexImageFilter->SetInput1(realImage);
  realAndImaginaryToComplexImageFilter->SetInput2(imaginaryImage);
  realAndImaginaryToComplexImageFilter->Update();

  ComplexImageType* output = realAndImaginaryToComplexImageFilter->GetOutput();
  output->Print(std::cout);

  return EXIT_SUCCESS;
}
