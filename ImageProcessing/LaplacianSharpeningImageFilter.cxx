#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkLaplacianSharpeningImageFilter.h"

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
  typedef itk::Image< float,  2 >   FloatImageType;

  typedef itk::ImageFileReader< FloatImageType >  readerType;
  readerType::Pointer reader = readerType::New();
  reader->SetFileName(inputFilename);

  typedef itk::LaplacianSharpeningImageFilter<FloatImageType, FloatImageType >  LaplacianSharpeningImageFilterType;
  LaplacianSharpeningImageFilterType::Pointer laplacianSharpeningImageFilter =
    LaplacianSharpeningImageFilterType::New();
  laplacianSharpeningImageFilter->SetInput( reader->GetOutput() );

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "LaplacianSharpeningImageFilter";
  viewer.AddImage(
    laplacianSharpeningImageFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();
  return EXIT_SUCCESS;
}
