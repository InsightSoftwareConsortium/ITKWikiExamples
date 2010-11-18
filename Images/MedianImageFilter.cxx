#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMedianImageFilter.h"

#include "QuickView.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImageFile [radius]" << std::endl;
    return EXIT_FAILURE;
    }
  double  = 2.0;

  // Parse command line arguments
  std::string inputFilename = argv[1];

  // Setup types
  typedef itk::Image< unsigned char, 2 >                 UnsignedCharImageType;
  typedef itk::ImageFileReader< UnsignedCharImageType >  readerType;
  typedef itk::MedianImageFilter<
    UnsignedCharImageType, UnsignedCharImageType >       filterType;

  // Create and setup a reader
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );

  // Create and setup a median filter
  filterType::Pointer medianFilter = filterType::New();
  medianFilter->SetInput( reader->GetOutput() );

  QuickView viewer;
  viewer.AddImage<UnsignedCharImageType>(reader->GetOutput());  
  viewer.AddImage<UnsignedCharImageType>(medianFilter->GetOutput());  
  viewer.Visualize();

  return EXIT_SUCCESS;
}
