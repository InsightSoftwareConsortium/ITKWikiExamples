#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageRandomNonRepeatingIteratorWithIndex.h"
#include "itkImageFileWriter.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

int main (int argc, char *argv[])
{
  if (argc < 3)
    {
    std::cerr << "Usage: " << argv[0]
              << " inputFile outputFile [percent]" << std::endl;
    return EXIT_FAILURE;
    }
  double percent = .1;
  if (argc > 3)
    {
    percent = atof(argv[3]);
    if (percent >= 1.0)
      {
      percent /= 100.0;
      }
    }
  typedef itk::Image<unsigned char, 2>  ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageRandomNonRepeatingIteratorWithIndex<ImageType> IteratorType;
  typedef itk::ImageFileWriter<ImageType> WriterType;

  // Read the binary file
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  // At x% of the pixels, add a uniform random value between 0 and 255
  IteratorType it(reader->GetOutput(),
                  reader->GetOutput()->GetLargestPossibleRegion());
  it.SetNumberOfSamples(reader->GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels() * percent);
  std::cout << "Number of random samples: "
            << it.GetNumberOfSamples() << std::endl;
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
  GeneratorType::Pointer random = GeneratorType::New();

  it.GoToBegin();
  while(!it.IsAtEnd())
    {
    it.Set(random->GetUniformVariate(0, 255));
    ++it;
    }   

  // Write the file
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
