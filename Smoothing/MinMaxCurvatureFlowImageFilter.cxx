#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkMinMaxCurvatureFlowImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "QuickView.h"

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
  std::string inputFilename = argv[1];

  typedef   float           PixelType;
  const     unsigned int    Dimension = 2;

  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef  itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::MinMaxCurvatureFlowImageFilter< ImageType, ImageType >
                                              MinMaxCurvatureFlowImageFilterType;
  typedef itk::SubtractImageFilter<ImageType> SubtractType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename );

  MinMaxCurvatureFlowImageFilterType::Pointer minMaxCurvatureFlowImageFilter = MinMaxCurvatureFlowImageFilterType::New();
  minMaxCurvatureFlowImageFilter->SetInput( reader->GetOutput() );
  minMaxCurvatureFlowImageFilter->SetNumberOfIterations( iterations );
  minMaxCurvatureFlowImageFilter->SetTimeStep( 0.125 );

  SubtractType::Pointer diff = SubtractType::New();
  diff->SetInput1(reader->GetOutput());
  diff->SetInput2(minMaxCurvatureFlowImageFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(inputFilename));  

  std::stringstream desc;
  desc << "MinMaxCurvatureFlow, iterations = " << iterations;
  viewer.AddImage(
    minMaxCurvatureFlowImageFilter->GetOutput(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Original - Median";
  viewer.AddImage(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
