#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMeanImageFilter.h"

#include "QuickView.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << argv[0] << " InputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  // Setup types
  typedef itk::Image< unsigned char, 2 >                UnsignedCharImageType;
  typedef itk::ImageFileReader< UnsignedCharImageType > ReaderType;
  typedef itk::MeanImageFilter<
    UnsignedCharImageType, UnsignedCharImageType > filterType;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  // Create and setup a mean filter
  filterType::Pointer meanFilter = filterType::New();
  meanFilter->SetInput( reader->GetOutput() );

  QuickView viewer;
  viewer.AddImage(reader->GetOutput());
  viewer.AddImage(meanFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
