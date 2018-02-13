#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#if ITK_VERSION_MAJOR < 4
#include "itkVnlFFTRealToComplexConjugateImageFilter.h"
#include "itkVnlFFTComplexConjugateToRealImageFilter.h"
#else
#include "itkVnlForwardFFTImageFilter.h"
#include "itkVnlInverseFFTImageFilter.h"
#endif
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkFFTShiftImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"

#if ITK_VERSION_MAJOR < 4
#include "itkMultiplyByConstantImageFilter.h"
#include "itkRealAndImaginaryToComplexImageFilter.h"
#else
#include "itkComposeImageFilter.h"
#endif

int main(int argc, char*argv[])
{
  constexpr unsigned int Dimension = 2;
  using PixelType = float;
  using FloatImageType = itk::Image< PixelType, Dimension >;
  using UnsignedCharImageType = itk::Image< unsigned char, Dimension >;

  if( argc < 3 )
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " FixedImage MovingImage"<< std::endl;;
    return EXIT_FAILURE;
    }
  std::string fixedImageFilename = argv[1];
  std::string movingImageFilename = argv[2];

  // Read the input images
  using ImageReaderType = itk::ImageFileReader< FloatImageType  >;
  ImageReaderType::Pointer  fixedImageReader  = ImageReaderType::New();
  fixedImageReader->SetFileName( fixedImageFilename );
  fixedImageReader->Update();
  
  ImageReaderType::Pointer movingImageReader = ImageReaderType::New();
  movingImageReader->SetFileName( movingImageFilename );
  movingImageReader->Update();

  // Shift the input images
  using FFTShiftFilterType = itk::FFTShiftImageFilter< FloatImageType, FloatImageType >;
  FFTShiftFilterType::Pointer fixedFFTShiftFilter = FFTShiftFilterType::New();
  fixedFFTShiftFilter->SetInput(fixedImageReader->GetOutput());
  fixedFFTShiftFilter->Update();

  FFTShiftFilterType::Pointer movingFFTShiftFilter = FFTShiftFilterType::New();
  movingFFTShiftFilter->SetInput(movingImageReader->GetOutput());
  movingFFTShiftFilter->Update();
  
  // Compute the FFT of the input
#if ITK_VERSION_MAJOR < 4
  using FFTFilterType = itk::VnlFFTRealToComplexConjugateImageFilter< FloatImageType >;
#else
  using FFTFilterType = itk::VnlForwardFFTImageFilter< FloatImageType >;
#endif
  FFTFilterType::Pointer fixedFFTFilter = FFTFilterType::New();
  fixedFFTFilter->SetInput( fixedFFTShiftFilter->GetOutput() );
  fixedFFTFilter->Update();
  
  FFTFilterType::Pointer movingFFTFilter = FFTFilterType::New();
  movingFFTFilter->SetInput( movingFFTShiftFilter->GetOutput() );
  
  using SpectralImageType = FFTFilterType::OutputImageType;

  // Take the conjugate of the fftFilterMoving
  // Extract the real part
  using RealFilterType = itk::ComplexToRealImageFilter<SpectralImageType, FloatImageType>;
  RealFilterType::Pointer realFilter = RealFilterType::New();
  realFilter->SetInput(movingFFTFilter->GetOutput());

  // Extract the imaginary part
  using ImaginaryFilterType = itk::ComplexToImaginaryImageFilter<SpectralImageType, FloatImageType>;
  ImaginaryFilterType::Pointer imaginaryFilter = ImaginaryFilterType::New();
  imaginaryFilter->SetInput(movingFFTFilter->GetOutput());

  // Flip the sign of the imaginary and combine with the real part again
#if ITK_VERSION_MAJOR < 4
  using MultiplyConstantFilterType = itk::MultiplyByConstantImageFilter<FloatImageType,PixelType,FloatImageType>;
#else
  using MultiplyConstantFilterType = itk::MultiplyImageFilter<FloatImageType,FloatImageType,FloatImageType>;
#endif

  MultiplyConstantFilterType::Pointer flipSignFilter = MultiplyConstantFilterType::New();
#if ITK_VERSION_MAJOR < 4
  flipSignFilter->SetConstant(-1);
#else
  flipSignFilter->SetConstant2(-1);
#endif
  flipSignFilter->SetInput(imaginaryFilter->GetOutput());
#if ITK_VERSION_MAJOR < 4
  using RealImageToComplexFilterType = itk::RealAndImaginaryToComplexImageFilter<FloatImageType>;
#else
  using RealImageToComplexFilterType = itk::ComposeImageFilter<FloatImageType, SpectralImageType>;
#endif
  RealImageToComplexFilterType::Pointer conjugateFilter = RealImageToComplexFilterType::New();
  conjugateFilter->SetInput1(realFilter->GetOutput());
  conjugateFilter->SetInput2(flipSignFilter->GetOutput());

  // The conjugate product of the spectrum
  using MultiplyFilterType = itk::MultiplyImageFilter< SpectralImageType,
    SpectralImageType,
    SpectralImageType >;
  MultiplyFilterType::Pointer multiplyFilter = MultiplyFilterType::New();
  multiplyFilter->SetInput1( fixedFFTFilter->GetOutput() );
  multiplyFilter->SetInput2( conjugateFilter->GetOutput() );

  // IFFT
#if ITK_VERSION_MAJOR < 4
  using IFFTFilterType = itk::VnlFFTComplexConjugateToRealImageFilter< SpectralImageType >;
#else
  using IFFTFilterType = itk::VnlInverseFFTImageFilter< SpectralImageType >;
#endif
  IFFTFilterType::Pointer fftInverseFilter = IFFTFilterType::New();
  fftInverseFilter->SetInput( multiplyFilter->GetOutput() );

  // Write the spectrum
  using RescaleFilterType = itk::RescaleIntensityImageFilter< FloatImageType,  UnsignedCharImageType >;
  RescaleFilterType::Pointer  rescaler =  RescaleFilterType::New();
  rescaler->SetInput( fftInverseFilter->GetOutput() );
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->Update();
    
  using WriterType = itk::ImageFileWriter< UnsignedCharImageType >;
  WriterType::Pointer writer =  WriterType::New();
  writer->SetFileName( "CrossCorr.png" );
  writer->SetInput( rescaler->GetOutput() );
  writer->Update();

  using ImageCalculatorFilterType = itk::MinimumMaximumImageCalculator <UnsignedCharImageType>;

  ImageCalculatorFilterType::Pointer imageCalculatorFilter
    = ImageCalculatorFilterType::New ();
  imageCalculatorFilter->SetImage(rescaler->GetOutput());
  imageCalculatorFilter->Compute();
  
  UnsignedCharImageType::IndexType maximumLocation = imageCalculatorFilter->GetIndexOfMaximum();
  std::cout << maximumLocation << std::endl; // should be (17,15)
  
  /*
  if ypeak < size(I,1)/2 ypeak = -(ypeak-1);
  else ypeak = size(I,1) - (ypeak-1);
  end
  if xpeak < size(I,2)/2 xpeak = -(xpeak-1);
  else xpeak = size(I,2) - (xpeak-1);
  end
  */
  
  return EXIT_SUCCESS;
}
