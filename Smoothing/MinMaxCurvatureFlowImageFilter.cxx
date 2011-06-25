#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkMinMaxCurvatureFlowImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main( int argc, char *argv[])
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage [iterations]" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFileName = argv[1];
  
  int iterations = 5;
  if (argc > 2)
    {
    std::stringstream ss(argv[2]);
    ss >> iterations;
    }

  typedef   float           InternalPixelType;
  const     unsigned int    Dimension = 2;
  typedef itk::Image< InternalPixelType, Dimension >  ImageType;

  typedef  itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::MinMaxCurvatureFlowImageFilter< ImageType, ImageType > MinMaxCurvatureFlowImageFilterType;
  MinMaxCurvatureFlowImageFilterType::Pointer minMaxCurvatureFlowImageFilter = MinMaxCurvatureFlowImageFilterType::New();
  minMaxCurvatureFlowImageFilter->SetInput( reader->GetOutput() );
  minMaxCurvatureFlowImageFilter->SetNumberOfIterations( iterations );
  minMaxCurvatureFlowImageFilter->SetTimeStep( 0.125 );
  minMaxCurvatureFlowImageFilter->Update();

  return EXIT_SUCCESS;
}
