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

#include "itkConnectedComponentImageFilter.h"
#include "itkLabelToRGBImageFilter.h"
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

  typedef  short                       InputPixelType;
  typedef  int                         OutputPixelType;
  typedef itk::RGBPixel<unsigned char> RGBPixelType;

  typedef itk::Image< InputPixelType,  2 > InputImageType;
  typedef itk::Image< OutputPixelType, 2 > OutputImageType;
  typedef itk::Image<RGBPixelType, 2>      RGBImageType;

  typedef itk::LiThresholdImageFilter<InputImageType, OutputImageType >
    LiFilterType;
  typedef itk::HuangThresholdImageFilter<InputImageType, OutputImageType >
    HuangFilterType;
  typedef itk::IntermodesThresholdImageFilter<InputImageType, OutputImageType >
    IntermodesFilterType;
  typedef itk::IsoDataThresholdImageFilter<InputImageType, OutputImageType >
    IsoDataFilterType;
  typedef itk::KittlerIllingworthThresholdImageFilter<InputImageType, OutputImageType >
    KittlerIllingworthFilterType;
  typedef itk::LiThresholdImageFilter<InputImageType, OutputImageType >
    LiFilterType;
  typedef itk::MaximumEntropyThresholdImageFilter<InputImageType, OutputImageType >
    MaximumEntropyFilterType;
  typedef itk::MomentsThresholdImageFilter<InputImageType, OutputImageType >
    MomentsFilterType;
  typedef itk::OtsuThresholdImageFilter<InputImageType, OutputImageType >
    OtsuFilterType;
  typedef itk::RenyiEntropyThresholdImageFilter<InputImageType, OutputImageType >
    RenyiEntropyFilterType;
  typedef itk::ShanbhagThresholdImageFilter<InputImageType, OutputImageType >
    ShanbhagFilterType;
  typedef itk::TriangleThresholdImageFilter<InputImageType, OutputImageType >
    TriangleFilterType;
  typedef itk::YenThresholdImageFilter<InputImageType, OutputImageType >
    YenFilterType;

  typedef itk::ConnectedComponentImageFilter <OutputImageType, OutputImageType >
    ConnectedComponentImageFilterType;
  typedef itk::LabelToRGBImageFilter<OutputImageType, RGBImageType> RGBFilterType;

  typedef itk::ImageFileReader< InputImageType >  ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  typedef std::map<std::string, itk::HistogramThresholdImageFilter<InputImageType, OutputImageType>::Pointer> FilterContainerType;
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
    (*it).second->SetNumberOfHistogramBins( 25 );
    (*it).second->SetInput( reader->GetOutput() );
    try
      {
      (*it).second->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cout << "Caught exception" << std::endl;
      std::cout << err << std::endl;
      continue;
      }
    ConnectedComponentImageFilterType::Pointer connected =
      ConnectedComponentImageFilterType::New ();
    connected->SetInput((*it).second->GetOutput());

    RGBFilterType::Pointer rgbFilter = RGBFilterType::New();
    rgbFilter->SetInput( connected->GetOutput() );
    std::stringstream desc;
    desc << (*it).first << " threshold = "
         << (*it).second->GetThreshold();
    viewer.AddRGBImage(
      rgbFilter->GetOutput(),
      true,
      desc.str());  
    }

  viewer.Visualize();
  return EXIT_SUCCESS;
}
