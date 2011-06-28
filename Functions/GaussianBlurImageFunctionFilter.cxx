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
  
  typedef itk::Image< float, 2 >             ImageType;
  typedef itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::ImageRegionIterator< ImageType > IteratorType;
  typedef itk::ImageRegionConstIterator< ImageType > ConstIteratorType;
 
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
 
  typedef itk::GaussianBlurImageFunction< ImageType > GaussianBlurImageFunctionType;
  // SetFunctor seems to need an actual object (not a pointer), but we can't create one like this:
  //GaussianBlurImageFunctionType gaussianFunction;
  
  GaussianBlurImageFunctionType::Pointer gaussianFunction = GaussianBlurImageFunctionType::New();
  //gaussianFunction->SetInputImage( reader->GetOutput() ); // Do we need to do this since we are going to do unaryFunctor->SetInput?
  GaussianBlurImageFunctionType::ErrorArrayType setError;
  setError.Fill( 0.01 );
  gaussianFunction->SetMaximumError( setError );
  gaussianFunction->SetSigma( sigma );
  gaussianFunction->SetMaximumKernelWidth( maxKernelWidth );
    
  typedef itk::UnaryFunctorImageFilter<ImageType,ImageType,
                                  GaussianBlurImageFunctionType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetFunctor(*gaussianFunction);
  filter->Update();

  typedef itk::ImageFileWriter < ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(filter->GetOutput());
  writer->Update();
 
  return EXIT_SUCCESS;
}
