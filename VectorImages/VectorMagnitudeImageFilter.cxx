#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkVectorImage.h"
#include "itkVectorMagnitudeImageFilter.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];
  std::string outputFilename = argv[2];

  // Setup types
  using VectorImageType = itk::VectorImage< float,  2 >;
  using UnsignedCharImageType = itk::Image< unsigned char, 2 >;

  // Create and setup a reader
  using ReaderType = itk::ImageFileReader< VectorImageType >;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename );

  using WriterType = itk::ImageFileWriter< UnsignedCharImageType >;

  using VectorMagnitudeFilterType = itk::VectorMagnitudeImageFilter<
    VectorImageType, UnsignedCharImageType >;
  VectorMagnitudeFilterType::Pointer magnitudeFilter =
    VectorMagnitudeFilterType::New();
  magnitudeFilter->SetInput( reader->GetOutput() );

  // To write the magnitude image file, we should rescale the gradient values
  // to a reasonable range
  using rescaleFilterType = itk::RescaleIntensityImageFilter<
    UnsignedCharImageType, UnsignedCharImageType >;

  rescaleFilterType::Pointer rescaler =
    rescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput( magnitudeFilter->GetOutput() );
  
  WriterType::Pointer writer =
    WriterType::New();

  writer->SetFileName( outputFilename );
  writer->SetInput( rescaler->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
