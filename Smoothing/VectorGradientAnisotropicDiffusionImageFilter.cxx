#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkImageFileReader.h"
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkVectorToRGBImageAdaptor.h"
#include "itkRGBToVectorImageAdaptor.h"
#include "itkCastImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

int main( int argc, char *argv[])
{
  // Verify arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: "<< std::endl;
    std::cerr << argv[0];
    std::cerr << " InputFileName";
    std::cerr << " [NumberOfIterations] ";
    std::cerr << " [Conductance]" << std::endl;
    return EXIT_FAILURE;
    }
  
  // 0) Parse arguments
  std::string inputFileName = argv[1];

  typedef itk::Image< itk::Vector<float, 3>, 2 >  FloatImageType;
  typedef itk::Image< itk::RGBPixel<float>, 2 >   RGBImageType;
  
  // 1) Read the RGB image
  typedef  itk::ImageFileReader< RGBImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );

  // 2) Cast to Vector image for processing
  typedef itk::RGBToVectorImageAdaptor<RGBImageType> AdaptorInputType;
  AdaptorInputType::Pointer adaptInput = AdaptorInputType::New();
  adaptInput->SetImage(reader->GetOutput());
  typedef itk::CastImageFilter<AdaptorInputType,FloatImageType> CastInputType;
  CastInputType::Pointer castInput = CastInputType::New();
  castInput->SetInput(adaptInput);

  // 3) Smooth the image
  typedef itk::VectorGradientAnisotropicDiffusionImageFilter< FloatImageType, 
    FloatImageType > VectorGradientAnisotropicDiffusionImageFilterType;
  VectorGradientAnisotropicDiffusionImageFilterType::Pointer filter = 
    VectorGradientAnisotropicDiffusionImageFilterType::New();
  filter->SetInput( castInput->GetOutput() );
  filter->SetTimeStep(0.125);
  if (argc > 2)
    {
    filter->SetNumberOfIterations(atoi(argv[2]));
    }
  if (argc > 3)
    {
    filter->SetConductanceParameter(atof(argv[3]));
    }

  // 4) Cast the Vector image to an RGB image for display
  typedef itk::VectorToRGBImageAdaptor<FloatImageType> AdaptorOutputType;
  AdaptorOutputType::Pointer adaptOutput = AdaptorOutputType::New();
  adaptOutput->SetImage(filter->GetOutput());
  typedef itk::CastImageFilter<AdaptorOutputType,RGBImageType> CastOutputType;
  CastOutputType::Pointer castOutput = CastOutputType::New();
  castOutput->SetInput(adaptOutput);
  
  // 5) Display the input and smoothed images
  QuickView viewer;
  viewer.AddRGBImage(
    reader->GetOutput(),
    true,
    itksys::SystemTools::GetFilenameName(inputFileName));  

  std::stringstream desc;
  desc << "VectorGradientAnisotropicDiffusionImageFilter\niterations: "
       << filter->GetNumberOfIterations() << " conductance: "
       << filter->GetConductanceParameter();
  viewer.AddRGBImage(
    castOutput->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
