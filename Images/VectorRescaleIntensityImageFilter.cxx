#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVectorRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Required: input output" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  
  using FloatImageType = itk::Image<itk::CovariantVector<float, 3>, 2>;
  using UnsignedCharImageType = itk::Image<itk::CovariantVector<unsigned char, 3>, 2>;

  using ReaderType = itk::ImageFileReader<FloatImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  using VectorRescaleFilterType = itk::VectorRescaleIntensityImageFilter<FloatImageType, UnsignedCharImageType>;
  VectorRescaleFilterType::Pointer rescaleFilter = VectorRescaleFilterType::New();
  rescaleFilter->SetInput(reader->GetOutput());
  rescaleFilter->SetOutputMaximumMagnitude(255);
  rescaleFilter->Update();

  using WriterType = itk::ImageFileWriter<UnsignedCharImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}
