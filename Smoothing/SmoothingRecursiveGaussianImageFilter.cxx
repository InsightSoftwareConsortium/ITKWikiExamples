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
  typedef itk::Image<PixelComponentType, Dimension >
    ScalarImageType;
  typedef itk::ImageFileReader< ColorImageType >
    ReaderType;
  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::NthElementImageAdaptor<ColorImageType,
    PixelComponentType> ImageAdaptorType;

  typedef itk::SmoothingRecursiveGaussianImageFilter<
    ImageAdaptorType, ScalarImageType >  FilterType;

  ImageAdaptorType::Pointer adaptorR = ImageAdaptorType::New();
  adaptorR->SelectNthElement(0);
  adaptorR->SetImage(reader->GetOutput());

  FilterType::Pointer gaussianR = FilterType::New();
  gaussianR->SetInput(adaptorR);
  gaussianR->SetSigma(sigma);

  ImageAdaptorType::Pointer adaptorG = ImageAdaptorType::New();
  adaptorG->SelectNthElement(1);
  adaptorG->SetImage(reader->GetOutput());

  FilterType::Pointer gaussianG = FilterType::New();
  gaussianG->SetInput(adaptorG);
  gaussianG->SetSigma(sigma);

  ImageAdaptorType::Pointer adaptorB = ImageAdaptorType::New();
  adaptorB->SelectNthElement(2);
  adaptorB->SetImage(reader->GetOutput());

  FilterType::Pointer gaussianB = FilterType::New();
  gaussianB->SetInput(adaptorB);
  gaussianB->SetSigma(sigma);

  typedef itk::ComposeRGBImageFilter<
    ScalarImageType, ColorImageType >  ComposeFilterType;
  ComposeFilterType::Pointer composeFilter =
    ComposeFilterType::New();
  composeFilter->SetInput1(gaussianR->GetOutput());
  composeFilter->SetInput2(gaussianG->GetOutput());
  composeFilter->SetInput3(gaussianB->GetOutput());

  QuickView viewer;
  viewer.AddRGBImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "SmoothingRecursiveGaussian\n sigma = " << sigma;
  viewer.AddRGBImage(
    composeFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
