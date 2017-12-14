#include "itkImage.h"

#if ITK_VERSION_MAJOR < 4
#include "itkRealAndImaginaryToComplexImageFilter.h"
#else
#include "itkComposeImageFilter.h"
#endif

#include <complex>

int main( int /*argc*/, char * /*argv*/[] )
{
  typedef itk::Image<unsigned char, 2> ImageType;
  typedef itk::Image<std::complex<float>, 2> ComplexImageType;

  ImageType::Pointer realImage = ImageType::New();
  ImageType::Pointer imaginaryImage = ImageType::New();

#if ITK_VERSION_MAJOR < 4
  typedef itk::RealAndImaginaryToComplexImageFilter<ImageType,ComplexImageType> RealAndImaginaryToComplexImageFilterType;
#else
  typedef itk::ComposeImageFilter<ImageType,ComplexImageType> RealAndImaginaryToComplexImageFilterType;
#endif
  RealAndImaginaryToComplexImageFilterType::Pointer realAndImaginaryToComplexImageFilter = RealAndImaginaryToComplexImageFilterType::New();
  realAndImaginaryToComplexImageFilter->SetInput1(realImage);
  realAndImaginaryToComplexImageFilter->SetInput2(imaginaryImage);
  realAndImaginaryToComplexImageFilter->Update();

  ComplexImageType* output = realAndImaginaryToComplexImageFilter->GetOutput();
  output->Print(std::cout);

  return EXIT_SUCCESS;
}
