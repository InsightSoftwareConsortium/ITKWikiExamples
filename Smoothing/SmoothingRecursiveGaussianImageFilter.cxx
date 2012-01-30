#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkRGBPixel.h"
#include "itkNthElementImageAdaptor.h"
#include "itkComposeRGBImageFilter.h"

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
    sigma = atof(argv[2]);
    }


  const unsigned int Dimension = 2;
  typedef unsigned char PixelComponentType;

  typedef itk::Image<itk::RGBPixel< PixelComponentType>,
    Dimension > ColorImageType;

  typedef itk::ImageFileReader< ColorImageType >
    ReaderType;
  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::SmoothingRecursiveGaussianImageFilter<
    ColorImageType, ColorImageType >  FilterType;

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
