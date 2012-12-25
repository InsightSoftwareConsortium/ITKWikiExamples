#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkLaplacianSharpeningImageFilter.h"
#include "itkSubtractImageFilter.h"

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

  typedef itk::SubtractImageFilter<FloatImageType>           SubtractType;
  SubtractType::Pointer diff = SubtractType::New();
  diff->SetInput1(reader->GetOutput());
  diff->SetInput2(laplacianSharpeningImageFilter->GetOutput());

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

  std::stringstream desc2;
  desc2 << "Original - LaplacianSharpening";
  viewer.AddImage(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();
  return EXIT_SUCCESS;
}
