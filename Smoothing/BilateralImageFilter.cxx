#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkBilateralImageFilter.h"
#include "itkSubtractImageFilter.h"
 
#include "QuickView.h"
 
int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile [domainSigma] [rangeSigma]" << std::endl;
    return EXIT_FAILURE;
    }
  double rangeSigma = 2.0;
  double domainSigma = 2.0;
  if (argc > 2)
    {
    domainSigma = std::stod(argv[2]);
    }
  if (argc > 3)
    {
    rangeSigma = std::stod(argv[3]);
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];
 
  // Setup types
  using ImageType = itk::Image< float, 2 >;
 
  using ReaderType = itk::ImageFileReader< ImageType >;
  using FilterType = itk::BilateralImageFilter<
    ImageType, ImageType >;
  using SubtractImageFilterType = itk::SubtractImageFilter< ImageType >;
 
  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename.c_str() );
 
  // Create and setup a derivative filter
  FilterType::Pointer bilateralFilter = FilterType::New();
  bilateralFilter->SetInput( reader->GetOutput() );
  bilateralFilter->SetDomainSigma(domainSigma);
  bilateralFilter->SetRangeSigma(rangeSigma);

  SubtractImageFilterType::Pointer diff = SubtractImageFilterType::New();
  diff->SetInput1(reader->GetOutput());
  diff->SetInput2(bilateralFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "Bilateral\ndomainSigma = " << domainSigma
       << " rangeSigma = " << rangeSigma;
  viewer.AddImage(
    bilateralFilter->GetOutput(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Original - Bilateral";
  viewer.AddImage(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();
 
  return EXIT_SUCCESS;
}
