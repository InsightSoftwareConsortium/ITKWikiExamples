#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkCannyEdgeDetectionImageFilter.h"

#include "QuickView.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Usage: " << argv[0]
              << " Filename [variance lower_threshold upper_threshold]"
              << std::endl;
    return EXIT_FAILURE;
    }

  double variance = 2.0;
  double upperThreshold = 0.0;
  double lowerThreshold = 0.0;
  if (argc > 2)
    {
    variance = atof(argv[2]);
    }
  if (argc > 3)
    {
    lowerThreshold = atof(argv[3]);
    }
  if (argc > 4)
    {
    upperThreshold = atof(argv[4]);
    }

  typedef itk::Image<double, 2>  DoubleImageType;

  typedef itk::ImageFileReader<DoubleImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::CannyEdgeDetectionImageFilter <DoubleImageType, DoubleImageType>
    CannyEdgeDetectionImageFilterType;

  CannyEdgeDetectionImageFilterType::Pointer cannyFilter
    = CannyEdgeDetectionImageFilterType::New();
  cannyFilter->SetInput(reader->GetOutput());
  cannyFilter->SetVariance( variance );
  cannyFilter->SetUpperThreshold( upperThreshold );
  cannyFilter->SetLowerThreshold( lowerThreshold );

  QuickView viewer;
  viewer.AddImage<DoubleImageType>(reader->GetOutput());
  viewer.AddImage<DoubleImageType>(cannyFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
