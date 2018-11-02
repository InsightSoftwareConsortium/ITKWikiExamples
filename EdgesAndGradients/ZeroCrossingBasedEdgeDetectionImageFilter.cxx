#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkZeroCrossingBasedEdgeDetectionImageFilter.h"

#include "itksys/SystemTools.hxx"

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

  double var = 5.0;
  if (argc > 2)
    {
    var = std::stod(argv[2]);
    }

    // Parse command line arguments
  std::string inputFilename = argv[1];

  // Setup types
  using FloatImageType = itk::Image< float,  2 >;
  using ReaderType = itk::ImageFileReader< FloatImageType >;

  using FilterType = itk::ZeroCrossingBasedEdgeDetectionImageFilter<
    FloatImageType, FloatImageType >;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename.c_str() );

  // Create and setup a derivative filter
  FilterType::Pointer edgeDetector = FilterType::New();
  edgeDetector->SetInput( reader->GetOutput() );
  FilterType::ArrayType variance;
  variance.Fill(var);
  edgeDetector->SetVariance(variance);

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(inputFilename));  

  std::stringstream desc;
  desc << "ZeroBasedEdgeDetection, variance = "
       << edgeDetector->GetVariance();
  viewer.AddImage(
    edgeDetector->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
