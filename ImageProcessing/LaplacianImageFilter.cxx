#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkLaplacianImageFilter.h"

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

  using readerType = itk::ImageFileReader< FloatImageType >;
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );
  reader->Update();

  using filterType = itk::LaplacianImageFilter<FloatImageType, FloatImageType >;
  filterType::Pointer laplacianFilter = filterType::New();
  laplacianFilter->SetInput( reader->GetOutput() ); // NOTE: input image type must be double or float
  laplacianFilter->Update();

  QuickView viewer;
  viewer.AddImage<FloatImageType>(reader->GetOutput());
  viewer.AddImage<FloatImageType>(laplacianFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
