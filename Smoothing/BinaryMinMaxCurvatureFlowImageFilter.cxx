#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkBinaryMinMaxCurvatureFlowImageFilter.h"
#include "itkSubtractImageFilter.h"

#include "itksys/SystemTools.hxx"

#include "QuickView.h"

int main(int argc, char* argv[])
{
  if( argc < 2 )
    {
    std::cerr << argv[0] << " InputFileName [NumberOfIterations]" <<std ::endl;
    return EXIT_FAILURE;
    }

  std::string inputFileName = argv[1];

  unsigned int numberOfIterations = 2;
  if (argc > 2)
    {
    numberOfIterations = atoi(argv[2]);
    }

  const unsigned int Dimension = 2;

  typedef float InputPixelType;
  typedef float OutputPixelType;

  typedef itk::Image< InputPixelType, Dimension >   InputImageType;
  typedef itk::ImageFileReader< InputImageType >    ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

  typedef itk::BinaryMinMaxCurvatureFlowImageFilter< InputImageType, OutputImageType >    FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetThreshold( 255 );
  filter->SetNumberOfIterations(numberOfIterations);

  typedef itk::SubtractImageFilter<OutputImageType>           SubtractType;
  SubtractType::Pointer diff = SubtractType::New();
  diff->SetInput1(reader->GetOutput());
  diff->SetInput2(filter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(inputFileName));  

  std::stringstream desc;
  desc << "BinaryMinMaxCurvature, iterations = "
       << numberOfIterations;
  viewer.AddImage(
    filter->GetOutput(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Original - BinaryMinMaxCurvatureFlow";
  viewer.AddImage(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
