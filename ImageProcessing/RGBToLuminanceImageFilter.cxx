#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkNormalizeImageFilter.h"
#include "itkRGBToLuminanceImageFilter.h"

typedef itk::Image<itk::RGBPixel<unsigned char>, 2>  RGBImageType;
typedef itk::Image<float, 2>  ScalarImageType;

int main(int argc, char *argv[])
{
  // Verify arguments
  if(argc < 2)
    {
    std::cerr << "Required: input output" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse arguments
  std::string inputFileName = argv[1];
  std::string outputFileName = argv[2];
  
  typedef itk::ImageFileReader<RGBImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  typedef itk::RGBToLuminanceImageFilter< RGBImageType, ScalarImageType > LuminanceFilterType;
  LuminanceFilterType::Pointer luminanceFilter = LuminanceFilterType::New();
  luminanceFilter->SetInput(reader->GetOutput());
  luminanceFilter->Update();

  typedef itk::ImageFileWriter<ScalarImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(luminanceFilter->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}
