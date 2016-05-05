#include "itkMeanImageFilter.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRGBPixel.h"
#include "itkVector.h"
#include "itkVectorCastImageFilter.h"

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
  std::string inputFilename = argv[1];

  // Setup types
  typedef itk::Vector<float,3>                        PixelType;
  typedef itk::Image<PixelType, 2 >                   ImageType;
  typedef itk::RGBPixel<unsigned char>                RGBPixelType;
  typedef itk::Image<RGBPixelType, 2 >                RGBImageType;

  typedef itk::ImageFileReader<ImageType>                     ReaderType;
  typedef itk::MeanImageFilter<ImageType, ImageType >         FilterType;
  typedef itk::VectorCastImageFilter<ImageType, RGBImageType>       CastType;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename );

  FilterType::InputSizeType radius;
  radius.Fill(2);
  if (argc > 2)
    {
    radius.Fill(atoi(argv[2]));
    }

  // Create and setup a mean filter
  FilterType::Pointer meanFilter = FilterType::New();
  meanFilter->SetRadius(radius);
  meanFilter->SetInput(reader->GetOutput());

  // Cast the custom Vector's to RGBPixel's
  CastType::Pointer castReader = CastType::New();
  castReader->SetInput(reader->GetOutput());

  CastType::Pointer castMean = CastType::New();
  castMean->SetInput(meanFilter->GetOutput());

  QuickView viewer;
  viewer.AddRGBImage(
    castReader->GetOutput(),
    true,
    itksys::SystemTools::GetFilenameName(inputFilename));  

  std::stringstream desc;
  desc << "MeanImageFilter, radius = " << radius;
  viewer.AddRGBImage(
    castMean->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
