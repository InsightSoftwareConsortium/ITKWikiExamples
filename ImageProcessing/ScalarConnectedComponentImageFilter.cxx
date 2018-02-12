#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkLabelToRGBImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"

#include "itkScalarConnectedComponentImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

template <typename TImage>
static void CreateImage(TImage* const image);

template<typename TImage, typename TLabelImage>
static void SummarizeLabelStatistics (TImage* image,
                                      TLabelImage* labelImage);

int main( int argc, char *argv[])
{
  const unsigned int Dimension = 2;
  typedef short                                  PixelType;
  typedef itk::Image<PixelType, Dimension>       ImageType;

  typedef itk::RGBPixel<unsigned char>           RGBPixelType;
  typedef itk::Image<RGBPixelType, Dimension>    RGBImageType;

  typedef unsigned int                           LabelPixelType;
  typedef itk::Image<LabelPixelType, Dimension > LabelImageType;

  ImageType::Pointer image;
  PixelType distanceThreshold = 4;
  if( argc < 2 )
    {
    image = ImageType::New();
    CreateImage(image.GetPointer());
    }
  else
    {
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    if (argc > 2)
      {
      distanceThreshold = static_cast<PixelType> (atoi(argv[2]));
      }
    image = reader->GetOutput();
    }


  typedef itk::ScalarConnectedComponentImageFilter <ImageType, LabelImageType >
    ConnectedComponentImageFilterType;

  ConnectedComponentImageFilterType::Pointer connected =
    ConnectedComponentImageFilterType::New ();
  connected->SetInput(image);
  connected->SetDistanceThreshold(distanceThreshold);

  typedef itk::RelabelComponentImageFilter <LabelImageType, LabelImageType >
    RelabelFilterType;
  RelabelFilterType::Pointer relabel =
    RelabelFilterType::New();
  RelabelFilterType::ObjectSizeType minSize = 20;
  if (argc > 3)
    {
    minSize = atoi(argv[3]);
    }
  relabel->SetInput(connected->GetOutput());
  relabel->SetMinimumObjectSize(minSize);
  relabel->Update();

  SummarizeLabelStatistics (image.GetPointer(), relabel->GetOutput());

  typedef itk::LabelToRGBImageFilter<LabelImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbFilter =
    RGBFilterType::New();
  rgbFilter->SetInput( relabel->GetOutput() );

  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");  

  std::stringstream desc;
  desc << "Scalar Connected Components:\n# of Objects: "
       << relabel->GetNumberOfObjects()
       << " Threshold: "
       << itk::NumericTraits<ConnectedComponentImageFilterType::OutputPixelType>::PrintType(connected->GetDistanceThreshold())
       << " Min Size: " << relabel->GetMinimumObjectSize();
  viewer.AddRGBImage(
    rgbFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}

template <typename TImage>
void CreateImage(TImage* const image)
{
  // Create an image with 2 connected components
  typename TImage::IndexType start = {{0,0}};
  start[0] = 0;
  start[1] = 0;

  typename TImage::SizeType size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  typename TImage::RegionType region(start, size);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(typename TImage::IndexValueType r = 20; r < 80; r++)
    {
    for(typename TImage::IndexValueType c = 30; c < 100; c++)
      {
      typename TImage::IndexType pixelIndex = {{r,c}};

      image->SetPixel(pixelIndex, 255);
      }
    }

  // Make another square
  for(typename TImage::IndexValueType r = 100; r < 130; r++)
    {
    for(typename TImage::IndexValueType c = 115; c < 160; c++)
      {
      typename TImage::IndexType pixelIndex = {{r,c}};

      image->SetPixel(pixelIndex, 255);
      }
    }
}

template<typename TImage, typename TLabelImage>
void SummarizeLabelStatistics (TImage* image,
                               TLabelImage* labelImage)
{
  typedef itk::LabelStatisticsImageFilter< TImage, TLabelImage >
    LabelStatisticsImageFilterType;
  typename LabelStatisticsImageFilterType::Pointer labelStatisticsImageFilter =
    LabelStatisticsImageFilterType::New();
  labelStatisticsImageFilter->SetLabelInput( labelImage );
  labelStatisticsImageFilter->SetInput(image);
  labelStatisticsImageFilter->UseHistogramsOn(); // needed to compute median
  labelStatisticsImageFilter->Update();

  std::cout << "Number of labels: "
            << labelStatisticsImageFilter->GetNumberOfLabels() << std::endl;
  std::cout << std::endl;
  
  typedef typename LabelStatisticsImageFilterType::LabelPixelType LabelPixelType;

  for(auto vIt = labelStatisticsImageFilter->GetValidLabelValues().begin();
      vIt != labelStatisticsImageFilter->GetValidLabelValues().end();
      ++vIt)
    {
    if ( labelStatisticsImageFilter->HasLabel(*vIt) )
      {
      LabelPixelType labelValue = *vIt;
      std::cout << "Label: " << *vIt << std::endl;
      std::cout << "\tmin: "
                << labelStatisticsImageFilter->GetMinimum( labelValue )
                << std::endl;
      std::cout << "\tmax: "
                << labelStatisticsImageFilter->GetMaximum( labelValue )
                << std::endl;
      std::cout << "\tmedian: "
                << labelStatisticsImageFilter->GetMedian( labelValue )
                << std::endl;
      std::cout << "\tmean: "
                << labelStatisticsImageFilter->GetMean( labelValue )
                << std::endl;
      std::cout << "\tsigma: "
                << labelStatisticsImageFilter->GetSigma( labelValue )
                << std::endl;
      std::cout << "\tvariance: "
                << labelStatisticsImageFilter->GetVariance( labelValue )
                << std::endl;
      std::cout << "\tsum: "
                << labelStatisticsImageFilter->GetSum( labelValue )
                << std::endl;
      std::cout << "\tcount: "
                << labelStatisticsImageFilter->GetCount( labelValue )
                << std::endl;
      std::cout << "\tregion: "
                << labelStatisticsImageFilter->GetRegion( labelValue )
                << std::endl;
      }
    }

  }
