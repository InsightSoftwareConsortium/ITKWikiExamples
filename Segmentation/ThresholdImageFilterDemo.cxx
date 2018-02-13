#include "itkLiThresholdImageFilter.h"
#include "itkHuangThresholdImageFilter.h"
#include "itkIntermodesThresholdImageFilter.h"
#include "itkIsoDataThresholdImageFilter.h"
#include "itkKittlerIllingworthThresholdImageFilter.h"
#include "itkMaximumEntropyThresholdImageFilter.h"
#include "itkMomentsThresholdImageFilter.h"
#include "itkOtsuThresholdImageFilter.h"
#include "itkRenyiEntropyThresholdImageFilter.h"
#include "itkShanbhagThresholdImageFilter.h"
#include "itkTriangleThresholdImageFilter.h"
#include "itkYenThresholdImageFilter.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>
#include <map>
#include "QuickView.h"

int main(int argc, char* argv[] )
{
  if( argc < 2 )
    {
    std::cout << "Usage: " << argv[0];
    std::cout << " inputImageFile";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  using InputPixelType = short;
  using OutputPixelType = unsigned char;

  using InputImageType = itk::Image< InputPixelType,  2 >;
  using OutputImageType = itk::Image< OutputPixelType, 2 >;

  using LiFilterType = itk::LiThresholdImageFilter<InputImageType, OutputImageType >;
  using HuangFilterType = itk::HuangThresholdImageFilter<InputImageType, OutputImageType >;
  using IntermodesFilterType = itk::IntermodesThresholdImageFilter<InputImageType, OutputImageType >;
  using IsoDataFilterType = itk::IsoDataThresholdImageFilter<InputImageType, OutputImageType >;
  using KittlerIllingworthFilterType = itk::KittlerIllingworthThresholdImageFilter<InputImageType, OutputImageType >;
  using LiFilterType = itk::LiThresholdImageFilter<InputImageType, OutputImageType >;
  using MaximumEntropyFilterType = itk::MaximumEntropyThresholdImageFilter<InputImageType, OutputImageType >;
  using MomentsFilterType = itk::MomentsThresholdImageFilter<InputImageType, OutputImageType >;
  using OtsuFilterType = itk::OtsuThresholdImageFilter<InputImageType, OutputImageType >;
  using RenyiEntropyFilterType = itk::RenyiEntropyThresholdImageFilter<InputImageType, OutputImageType >;
  using ShanbhagFilterType = itk::ShanbhagThresholdImageFilter<InputImageType, OutputImageType >;
  using TriangleFilterType = itk::TriangleThresholdImageFilter<InputImageType, OutputImageType >;
  using YenFilterType = itk::YenThresholdImageFilter<InputImageType, OutputImageType >;

  using ReaderType = itk::ImageFileReader< InputImageType >;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  using FilterContainerType = std::map<std::string, itk::HistogramThresholdImageFilter<InputImageType, OutputImageType>::Pointer>;
  FilterContainerType filterContainer;

  filterContainer["Huang"] = HuangFilterType::New();
  filterContainer["Intermodes"] = IntermodesFilterType::New();
  filterContainer["IsoData"] = IsoDataFilterType::New();
  filterContainer["KittlerIllingworth"] = KittlerIllingworthFilterType::New();
  filterContainer["Li"] = LiFilterType::New();
  filterContainer["MaximumEntropy"] = MaximumEntropyFilterType::New();
  filterContainer["Moments"] = MomentsFilterType::New();
  filterContainer["Otsu"] = OtsuFilterType::New();
  filterContainer["RenyiEntropy"] = RenyiEntropyFilterType::New();
  filterContainer["Shanbhag"] = ShanbhagFilterType::New();
  filterContainer["Triangle"] = TriangleFilterType::New();
  filterContainer["Yen"] = YenFilterType::New();

  auto it = filterContainer.begin();
  for (it = filterContainer.begin(); it != filterContainer.end(); ++it)
    {
    (*it).second->SetInsideValue( 255 );
    (*it).second->SetOutsideValue( 0 );
    (*it).second->SetInput( reader->GetOutput() );
    (*it).second->SetNumberOfHistogramBins( 100 );
    (*it).second->Update();
    std::stringstream desc;
    desc << (*it).first << " threshold = "
         << (*it).second->GetThreshold();
    viewer.AddImage(
      (*it).second->GetOutput(),
      true,
      desc.str());  
    }


  viewer.Visualize();
  return EXIT_SUCCESS;
}
