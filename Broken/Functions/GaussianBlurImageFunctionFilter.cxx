#include "itkGaussianBlurImageFunction.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkUnaryFunctorImageFilter.h"
 
int main( int argc, char * argv[] )
{
  // Verify arguments
  if( argc < 5 ) 
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile sigma maxKernelWidth" << std::endl;
    return EXIT_FAILURE;
    }
 
  // Parse arguments
  std::string inputFileName = argv[1];
  std::string outputFileName = argv[2];
  float sigma = 0;
  {
  std::stringstream ss(argv[3]);
  ss >> sigma;
  }
  int maxKernelWidth = 0;
  {
  std::stringstream ss(argv[4]);
  ss >> maxKernelWidth;
  }
 
  using ImageType = itk::Image< float, 2 >;
  using ReaderType = itk::ImageFileReader< ImageType >;
  using IteratorType = itk::ImageRegionIterator< ImageType >;
  using ConstIteratorType = itk::ImageRegionConstIterator< ImageType >;
 
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );
  reader->Update();
 
  /*
  // This manually creates the output and computes the values at each pixel
  const ImageType * inputImage = reader->GetOutput();
 
  ImageType::RegionType region = inputImage->GetBufferedRegion();
 
  ImageType::Pointer output = ImageType::New();
 
  output->SetRegions( region );
  output->SetOrigin(  inputImage->GetOrigin()  );
  output->SetSpacing( inputImage->GetSpacing() );
  output->Allocate();
 
  ConstIteratorType it( inputImage, region );
  IteratorType out( output, region );
 
  it.GoToBegin();
  out.GoToBegin();
 
  while( !it.IsAtEnd() )
    {
    out.Set( gaussianFunction->EvaluateAtIndex(it.GetIndex() ) );
    ++it;
    ++out;
    }
  */
 
  using GaussianBlurImageFunctionType = itk::GaussianBlurImageFunction< ImageType >;
  // SetFunctor seems to need an actual object (not a pointer), but we can't create one like this:
  //GaussianBlurImageFunctionType gaussianFunction;
 
  GaussianBlurImageFunctionType::Pointer gaussianFunction = GaussianBlurImageFunctionType::New();
  //gaussianFunction->SetInputImage( reader->GetOutput() ); // Do we need to do this since we are going to do unaryFunctor->SetInput?

 
  using FilterType = itk::UnaryFunctorImageFilter<ImageType,ImageType,
                                  GaussianBlurImageFunctionType>;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  
  GaussianBlurImageFunctionType::ErrorArrayType setError;
  setError.Fill( 0.01 );
  filter->GetFunctor().SetMaximumError( setError );
  filter->GetFunctor().SetSigma( sigma );
  filter->GetFunctor().SetMaximumKernelWidth( maxKernelWidth );

  filter->Update();
 
  using WriterType = itk::ImageFileWriter < ImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(filter->GetOutput());
  writer->Update();
 
  return EXIT_SUCCESS;
}
