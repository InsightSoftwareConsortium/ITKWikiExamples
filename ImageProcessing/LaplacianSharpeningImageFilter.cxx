#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkLaplacianSharpeningImageFilter.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "inputImageFile output" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];
  
  // Output argument
  std::cout << "Input: " << inputFilename << std::endl;
  std::cout << "Input: " << outputFilename << std::endl;
  

  // Setup types
  typedef itk::Image< float,  2 >   FloatImageType;

  typedef itk::ImageFileReader< FloatImageType >  readerType;
  readerType::Pointer reader = readerType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  typedef itk::LaplacianSharpeningImageFilter<FloatImageType, FloatImageType >  LaplacianSharpeningImageFilterType;
  LaplacianSharpeningImageFilterType::Pointer laplacianSharpeningImageFilter = LaplacianSharpeningImageFilterType::New();
  laplacianSharpeningImageFilter->SetInput( reader->GetOutput() );
  laplacianSharpeningImageFilter->Update();

  typedef itk::ImageFileWriter< FloatImageType >  ImageFileWriterType;
  ImageFileWriterType::Pointer writer = ImageFileWriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(laplacianSharpeningImageFilter->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}
