#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkImage.h"

int main(int argc, char* argv[])
{  
   // Verify number of parameters in command line
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputVectorImageFile " << std::endl;
    return EXIT_FAILURE;
    }

  using InputPixelType = signed short;
  using ComponentType = float;
  const   unsigned int          Dimension = 2;

  using OutputPixelType = itk::CovariantVector< ComponentType, 
								Dimension  >;

  using InputImageType = itk::Image< InputPixelType,  Dimension >;
  using OutputImageType = itk::Image< OutputPixelType, Dimension >;

  using ReaderType = itk::ImageFileReader< InputImageType  >;
  using WriterType = itk::ImageFileWriter< OutputImageType >;
 
  //Filter class is instantiated
  using FilterType = itk::GradientRecursiveGaussianImageFilter<InputImageType, OutputImageType>;

  FilterType::Pointer filter = FilterType::New();
	
  //sigma is specified in millimeters
  filter->SetSigma( 1.5 );  
	
  //create the reader and writer 
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
	
  //recover file names from command line arguments
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];		//e.g. *.mhd (can be displayed in Paraview)

  reader->SetFileName( inputFilename  );
  writer->SetFileName(outputFilename);

  //  processing pipeline:
  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetOutput() );
 
  try 
    { 
    //execute the pipeline
    writer->Update(); 
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}
