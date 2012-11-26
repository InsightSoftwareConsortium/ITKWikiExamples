#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryMinMaxCurvatureFlowImageFilter.h"

int main(int argc, char* argv[])
{
  if( argc != 4 )
    {
    std::cerr << argv[0] << " InputFileName OutputFileName NumberOfIterations" <<std ::endl;
    return EXIT_FAILURE;
    }

  std::string inputFileName = argv[1];
  std::string outputFileName = argv[2];
  std::stringstream ssNumIter;
  ssNumIter << argv[3];
  unsigned int numberOfIterations;
  ssNumIter >> numberOfIterations;

  const unsigned int Dimension = 3;

  typedef unsigned char InputPixelType;
  typedef float         OutputPixelType;

  typedef itk::Image< InputPixelType, Dimension >   InputImageType;
  typedef itk::ImageFileReader< InputImageType >    ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

  typedef itk::BinaryMinMaxCurvatureFlowImageFilter< InputImageType, OutputImageType >    FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetThreshold( 255 );
  filter->SetNumberOfIterations(numberOfIterations);
  filter->Update();

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName(outputFileName);
  writer->Update();

  return EXIT_SUCCESS;
}
