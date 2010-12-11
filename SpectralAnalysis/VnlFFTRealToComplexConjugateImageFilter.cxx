#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVnlFFTRealToComplexConjugateImageFilter.h"
#include "itkVnlFFTComplexConjugateToRealImageFilter.h"
#include "itkMultiplyImageFilter.h"


int main( int argc, char * argv [] )
{
  if( argc < 4 )
    {
    std::cerr << "Usage: " << argv[0] << " inputScalarImage  inputMaskImage";
    std::cerr << " outputFilteredImage" << std::endl;
    }

  typedef float  InputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image< InputPixelType, Dimension > InputImageType;

  typedef itk::ImageFileReader< InputImageType >    InputReaderType;

  InputReaderType::Pointer inputReader1 = InputReaderType::New();
  InputReaderType::Pointer inputReader2 = InputReaderType::New();

  inputReader1->SetFileName( argv[1] );
  inputReader2->SetFileName( argv[2] );

  typedef itk::VnlFFTRealToComplexConjugateImageFilter<
                                  InputPixelType, Dimension >  FFTFilterType;

  FFTFilterType::Pointer fftFilter1 = FFTFilterType::New();
  FFTFilterType::Pointer fftFilter2 = FFTFilterType::New();

  fftFilter1->SetInput( inputReader1->GetOutput() );
  fftFilter2->SetInput( inputReader2->GetOutput() );

  typedef FFTFilterType::OutputImageType    SpectralImageType;

  typedef itk::MultiplyImageFilter< SpectralImageType,
                                    SpectralImageType,
                                    SpectralImageType >  MultiplyFilterType;

  MultiplyFilterType::Pointer multiplyFilter = MultiplyFilterType::New();

  multiplyFilter->SetInput1( fftFilter1->GetOutput() );
  multiplyFilter->SetInput2( fftFilter2->GetOutput() );

  typedef itk::VnlFFTComplexConjugateToRealImageFilter<
    InputPixelType, Dimension >  IFFTFilterType;

  IFFTFilterType::Pointer fftInverseFilter = IFFTFilterType::New();

  fftInverseFilter->SetInput( multiplyFilter->GetOutput() );

  typedef itk::ImageFileWriter< InputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( fftInverseFilter->GetOutput() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Error writing the real image: " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
