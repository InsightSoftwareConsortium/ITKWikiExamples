#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkBinomialBlurImageFilter.h"

#include "QuickView.h"
int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile [repetitions]" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];

  int repetitions = 2;
  if (argc > 2)
    {
    repetitions = std::stoi(argv[2]);
    }

  // Setup types
  using UnsignedCharImageType = itk::Image< unsigned char, 2 >;

  using readerType = itk::ImageFileReader< UnsignedCharImageType >;

  using filterType = itk::BinomialBlurImageFilter<
		  UnsignedCharImageType, UnsignedCharImageType >;

  // Create and setup a reader
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );

  // Create and setup a mean filter
  filterType::Pointer blurFilter = filterType::New();
  blurFilter->SetInput( reader->GetOutput() );
  blurFilter->SetRepetitions( repetitions );

  QuickView viewer;
  viewer.AddImage<UnsignedCharImageType>(reader->GetOutput());
  viewer.AddImage<UnsignedCharImageType>(blurFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
