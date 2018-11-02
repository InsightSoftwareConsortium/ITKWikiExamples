#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkRGBPixel.h"
#include "itkNthElementImageAdaptor.h"

#include "QuickView.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile [sigma]" << std::endl;
    return EXIT_FAILURE;
    }

  double sigma = 4.0;
  if (argc > 2)
    {
    sigma = std::stod(argv[2]);
    }


  constexpr unsigned int Dimension = 2;
  using PixelComponentType = unsigned char;

  using ColorImageType = itk::Image<itk::RGBPixel< PixelComponentType>,
    Dimension >;

  using ReaderType = itk::ImageFileReader< ColorImageType >;
  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  using FilterType = itk::SmoothingRecursiveGaussianImageFilter<
    ColorImageType, ColorImageType >;

  FilterType::Pointer smoothingRecursiveGaussianImageFilter = FilterType::New();
  smoothingRecursiveGaussianImageFilter->SetInput(reader->GetOutput());
  smoothingRecursiveGaussianImageFilter->SetSigma(sigma);
  smoothingRecursiveGaussianImageFilter->Update();

  QuickView viewer;
  viewer.AddRGBImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));

  std::stringstream desc;
  desc << "SmoothingRecursiveGaussian\n sigma = " << sigma;
  viewer.AddRGBImage(
    smoothingRecursiveGaussianImageFilter->GetOutput(),
    true,
    desc.str());

  viewer.Visualize();

  return EXIT_SUCCESS;
}
