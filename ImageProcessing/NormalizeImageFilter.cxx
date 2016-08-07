#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkNormalizeImageFilter.h"
#include "itkStatisticsImageFilter.h"

#include "QuickView.h"

#include <iomanip>

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::Image<double, 2>  FloatImageType;

  typedef itk::ImageFileReader<FloatImageType>
    ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::NormalizeImageFilter< FloatImageType, FloatImageType >
    NormalizeFilterType;
  NormalizeFilterType::Pointer normalizeFilter = NormalizeFilterType::New();
  normalizeFilter->SetInput(reader->GetOutput());

  typedef itk::StatisticsImageFilter< FloatImageType >
    StatisticsFilterType;
  StatisticsFilterType::Pointer statistics1 = StatisticsFilterType::New();
  statistics1->SetInput(reader->GetOutput());

  StatisticsFilterType::Pointer statistics2 = StatisticsFilterType::New();
  statistics2->SetInput(normalizeFilter->GetOutput());

  QuickView viewer;

  std::stringstream desc1;
  statistics1->Update();
  desc1 << itksys::SystemTools::GetFilenameName(argv[1])
        << "\nMean: " << statistics1->GetMean()
        << " Variance: " << statistics1->GetVariance();
  viewer.AddImage(
    reader->GetOutput(),
    true,
    desc1.str());  

  std::stringstream desc2;
  statistics2->Update();
  desc2 << "Normalize"
        << "\nMean: "
        << std::fixed << std::setprecision (2) << statistics2->GetMean()
        << " Variance: " << statistics2->GetVariance();
  viewer.AddImage(
    normalizeFilter->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
