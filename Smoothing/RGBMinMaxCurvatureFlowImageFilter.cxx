#include "itkImageAdaptor.h"
#include "itkImageRegionIterator.h"
#include "itkNthElementImageAdaptor.h"
#include "itkMinMaxCurvatureFlowImageFilter.h"
#include "itkComposeImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRGBPixel.h"

#include <sstream>

#include "QuickView.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImageFile [iterations]" << std::endl;
    return EXIT_FAILURE;
    }

  int iterations = 5;
  if (argc > 2)
    {
    std::stringstream ss(argv[2]);
    ss >> iterations;
    }
  std::string inputFilename = argv[1];

  // Setup types
  typedef float                                                    ComponentType;
  typedef itk::RGBPixel<ComponentType>                             PixelType;
  typedef itk::Image<PixelType, 2 >                                RGBImageType;
  typedef itk::Image<ComponentType, 2 >                            ImageType;
  typedef itk::ImageFileReader<RGBImageType>                       ReaderType;
  typedef itk::NthElementImageAdaptor<RGBImageType, unsigned char> ImageAdaptorType;
  typedef itk::ComposeImageFilter<ImageType, RGBImageType>         ComposeType;
  typedef itk::MinMaxCurvatureFlowImageFilter<ImageAdaptorType, ImageType>  CurvatureFlowType;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename );
  reader->Update();

  // Run the filter for each component
  ImageAdaptorType::Pointer rAdaptor =
    ImageAdaptorType::New();
  rAdaptor->SelectNthElement(0);
  rAdaptor->SetImage(reader->GetOutput());

  CurvatureFlowType::Pointer rCurvatureFilter =
    CurvatureFlowType::New();
  rCurvatureFilter->SetInput(rAdaptor);
  rCurvatureFilter->SetNumberOfIterations(iterations);
  rCurvatureFilter->Update();
  
  ImageAdaptorType::Pointer gAdaptor =
    ImageAdaptorType::New();
  gAdaptor->SelectNthElement(1);
  gAdaptor->SetImage(reader->GetOutput());

  CurvatureFlowType::Pointer gCurvatureFilter =
    CurvatureFlowType::New();
  gCurvatureFilter->SetInput(gAdaptor);
  gCurvatureFilter->SetNumberOfIterations(iterations);
  gCurvatureFilter->Update();
  
  ImageAdaptorType::Pointer bAdaptor =
    ImageAdaptorType::New();
  bAdaptor->SelectNthElement(2);
  bAdaptor->SetImage(reader->GetOutput());

  CurvatureFlowType::Pointer bCurvatureFilter =
    CurvatureFlowType::New();
  bCurvatureFilter->SetInput(bAdaptor);
  bCurvatureFilter->SetNumberOfIterations(iterations);
  bCurvatureFilter->Update();

  // compose an RGB image from the three filtered images

  ComposeType::Pointer compose =  
    ComposeType::New();
  compose->SetInput1 (rCurvatureFilter->GetOutput());
  compose->SetInput2 (gCurvatureFilter->GetOutput());
  compose->SetInput3 (bCurvatureFilter->GetOutput());
  
  QuickView viewer;
  viewer.AddRGBImage(
    reader->GetOutput(),
    true,
    itksys::SystemTools::GetFilenameName(inputFilename));  

  std::stringstream desc;
  desc << "MinMaxCurvatureFlow iterations = " << iterations;
  viewer.AddRGBImage(
    compose->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
