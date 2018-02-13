#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkGaussianBlurImageFunction.h"
#include "itkImageRegionIterator.h"

int main( int argc, char * argv[] )
{
  if( argc < 5 ) {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile sigma maxKernelWidth" << std::endl;
    return EXIT_FAILURE;
  }

  using ImageType = itk::Image< float, 2 >;
  using ReaderType = itk::ImageFileReader< ImageType >;
  using IteratorType = itk::ImageRegionIterator< ImageType >;
  using ConstIteratorType = itk::ImageRegionConstIterator< ImageType >;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  const ImageType * inputImage = reader->GetOutput();

  ImageType::RegionType region = inputImage->GetBufferedRegion();

  ConstIteratorType it( inputImage, region );

  ImageType::Pointer output = ImageType::New();

  output->SetRegions( region );
  output->SetOrigin(  inputImage->GetOrigin()  );
  output->SetSpacing( inputImage->GetSpacing() );
  output->Allocate();

  IteratorType out( output, region );

  using GFunctionType = itk::GaussianBlurImageFunction< ImageType >;
  GFunctionType::Pointer gaussianFunction = GFunctionType::New();
  gaussianFunction->SetInputImage( inputImage );

  GFunctionType::ErrorArrayType setError;
  setError.Fill( 0.01 );
  gaussianFunction->SetMaximumError( setError );
  gaussianFunction->SetSigma( atof( argv[3] ) );
  gaussianFunction->SetMaximumKernelWidth( atoi( argv[4] ) );

  it.GoToBegin();
  out.GoToBegin();

  while( !it.IsAtEnd() )
    {
    out.Set( gaussianFunction->EvaluateAtIndex(it.GetIndex() ) );
    ++it;
    ++out;
    }

  using WriterType = itk::ImageFileWriter < ImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
