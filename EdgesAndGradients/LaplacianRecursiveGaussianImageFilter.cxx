#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkLaplacianRecursiveGaussianImageFilter.h"

#include "QuickView.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];

  // Setup types
  using FloatImageType = itk::Image< float,  2 >;
  using UnsignedCharImageType = itk::Image< unsigned char, 2 >;

  using readerType = itk::ImageFileReader< UnsignedCharImageType >;

  using filterType = itk::LaplacianRecursiveGaussianImageFilter<
    UnsignedCharImageType, FloatImageType >;

  // Create and setup a reader
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );

  // Create and setup a derivative filter
  filterType::Pointer laplacianFilter = filterType::New();
  laplacianFilter->SetInput( reader->GetOutput() );

  QuickView viewer;
  viewer.AddImage<UnsignedCharImageType>(reader->GetOutput());
  viewer.AddImage<FloatImageType>(laplacianFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
