#include "itkImage.h"
#include "itkThresholdImageFilter.h"
#include "itkImageFileReader.h"

#include "QuickView.h"


int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Usage: ";
    std::cerr << argv[0] << " inputImageFile [lowerThreshold] [upperThreshold]" << std::endl;
    return EXIT_FAILURE;
    }

  int lowerThreshold = 10;
  int upperThreshold = 30;
  if (argc > 2)
    {
    lowerThreshold = atoi(argv[2]);
    }
  if (argc > 3)
    {
    upperThreshold = atoi(argv[3]);
    }

  typedef itk::Image<unsigned char, 2>    ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::ThresholdImageFilter <ImageType>
    ThresholdImageFilterType;

  ThresholdImageFilterType::Pointer thresholdFilter
    = ThresholdImageFilterType::New();
  thresholdFilter->SetInput(reader->GetOutput());
  thresholdFilter->ThresholdOutside(lowerThreshold, upperThreshold);
  thresholdFilter->SetOutsideValue(0);

  QuickView viewer;
  viewer.AddImage<ImageType>(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  
  std::stringstream desc;
  desc << "Threshold\nlower = " << lowerThreshold
       << " upper = " << upperThreshold;
  viewer.AddImage<ImageType>(
    thresholdFilter->GetOutput(),
    true,
    desc.str());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
