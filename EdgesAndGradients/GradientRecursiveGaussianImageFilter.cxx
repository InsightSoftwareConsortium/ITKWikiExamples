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

  //typedef
  typedef signed short          InputPixelType;
  typedef float                 ComponentType;
  const   unsigned int          Dimension = 2;

  typedef itk::CovariantVector< ComponentType, 
								Dimension  >      OutputPixelType;

  typedef itk::Image< InputPixelType,  Dimension >    InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >    OutputImageType;

  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;
 
  //Filter class is instantiated
  typedef itk::GradientRecursiveGaussianImageFilter<InputImageType, OutputImageType> FilterType;

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
