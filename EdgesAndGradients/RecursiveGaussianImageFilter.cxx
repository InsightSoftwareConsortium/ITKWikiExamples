#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"

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
  typedef itk::Image< float,  2 >   FloatImageType;
  typedef itk::Image< unsigned char, 2 >   UnsignedCharImageType;

  typedef itk::ImageFileReader< UnsignedCharImageType >  readerType;

  typedef itk::RecursiveGaussianImageFilter<
		  UnsignedCharImageType, FloatImageType >  filterType;

  // Create and setup a reader
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );

  // Create and setup a gaussian filter
  filterType::Pointer gaussianFilter = filterType::New();
  gaussianFilter->SetInput( reader->GetOutput() );
  gaussianFilter->SetDirection(0); // "x" axis
  gaussianFilter->SetSecondOrder();

  QuickView viewer;
  viewer.AddImage<UnsignedCharImageType>(reader->GetOutput());
  viewer.AddImage<FloatImageType>(gaussianFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
