#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMedianImageFilter.h"
#include "itkSubtractImageFilter.h"

#include "itksys/SystemTools.hxx"

#include <sstream>

#include "QuickView.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImageFile [radius]" << std::endl;
    return EXIT_FAILURE;
    }
  unsigned int radius = 2;
  if (argc > 2)
    {
    radius = atoi(argv[2]);
    }

  std::string inputFilename = argv[1];

  // Setup types
  typedef itk::Image<float, 2 >                         ImageType;
  typedef itk::ImageFileReader<ImageType>               ReaderType;
  typedef itk::MedianImageFilter<ImageType, ImageType > FilterType;
  typedef itk::SubtractImageFilter<ImageType>           SubtractType;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename );

  // Create and setup a median filter
  FilterType::Pointer medianFilter = FilterType::New();
  FilterType::SizeType kernelRadius;
  kernelRadius.Fill(radius);
  medianFilter->SetRadius(radius);
  medianFilter->SetInput( reader->GetOutput() );

  SubtractType::Pointer diff = SubtractType::New();
  diff->SetInput1(reader->GetOutput());
  diff->SetInput2(medianFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(inputFilename));  

  std::stringstream desc;
  desc << "MedianImageFilter, radius = " << radius;
  viewer.AddImage(
    medianFilter->GetOutput(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Original - Median";
  viewer.AddImage(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
