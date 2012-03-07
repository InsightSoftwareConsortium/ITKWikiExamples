#include "itkImage.h"
#include "itkBinaryPruningImageFilter.h"
#include "itkImageFileReader.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkImageFileWriter.h"

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImageFile OutputImageFile [iteration]" << std::endl;
    return EXIT_FAILURE;
    }

  unsigned int iteration = 1;
  if (argc > 3)
    {
    iteration = atoi(argv[3]);
    }

  const unsigned Dimension = 3;
  typedef unsigned char                       PixelType;
  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageFileReader<ImageType>     ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  typedef itk::BinaryPruningImageFilter <ImageType, ImageType >
          BinaryPruningImageFilterType;

  BinaryPruningImageFilterType::Pointer pruneFilter
          = BinaryPruningImageFilterType::New();
  pruneFilter->SetInput(reader->GetOutput());
  pruneFilter->SetIteration(iteration);

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( pruneFilter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}
