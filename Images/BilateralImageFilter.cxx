#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkBilateralImageFilter.h"

#include "QuickView.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "inputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];

  // Setup types
  typedef itk::Image< unsigned char, 2 >   UnsignedCharImageType;

  typedef itk::ImageFileReader< UnsignedCharImageType >  readerType;

  typedef itk::BilateralImageFilter<
    UnsignedCharImageType, UnsignedCharImageType >  filterType;

  // Create and setup a reader
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );

  // Create and setup a derivative filter
  filterType::Pointer bilateralFilter = filterType::New();
  bilateralFilter->SetInput( reader->GetOutput() );

  QuickView viewer;
  viewer.AddImage(reader->GetOutput());
  viewer.AddImage(bilateralFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
