#include "itkImageToHistogramFilter.h"
#include "itkImage.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIteratorWithIndex.h"

typedef itk::RGBPixel<unsigned char> RGBPixelType;
typedef itk::Image< RGBPixelType, 2> RGBImageType;

void CreateImage(RGBImageType::Pointer image);

int main(int, char *[])
{
  const unsigned int MeasurementVectorSize = 3; // RGB

  RGBImageType::Pointer image = RGBImageType::New();
  CreateImage(image);

  typedef itk::Statistics::ImageToHistogramFilter< RGBImageType >         HistogramFilterType;
  typedef HistogramFilterType::HistogramMeasurementVectorType             HistogramMeasurementVectorType;
  typedef HistogramFilterType::InputHistogramMeasurementVectorObjectType  InputHistogramMeasurementVectorObjectType;
  typedef HistogramFilterType::InputBooleanObjectType                     InputBooleanObjectType;
  typedef HistogramFilterType::HistogramSizeType                          HistogramSizeType;
  typedef HistogramFilterType::HistogramType                              HistogramType;

  HistogramFilterType::Pointer filter = HistogramFilterType::New();
  filter->SetInput(image);
  filter->SetAutoMinimumMaximum(true);

  HistogramSizeType histogramSize( MeasurementVectorSize );

  histogramSize[0] = 256;  // number of bins for the Red   channel
  histogramSize[1] =   1;  // number of bins for the Green channel
  histogramSize[2] =   1;  // number of bins for the Blue  channel

  // Compute the bounds of the histrogram automatically
  filter->SetHistogramSize(histogramSize);
  filter->SetMarginalScale(10); // What is this?
  filter->Update();

  const HistogramType * histogram = filter->GetOutput();

  std::cout << "Histogram of the red component" << std::endl;
  for(unsigned int i = 0; i < histogramSize[0]; i++)
    {
    std::cout << histogram->GetFrequency(i, 0) << " "; // Red channel
    }
  std::cout << std::endl;
  return EXIT_SUCCESS;
}

void CreateImage(RGBImageType::Pointer image)
{
  // Create a black image with a red square and a green square.
  // This should produce a histogram with very strong spikes.
  RGBImageType::RegionType region;
  RGBImageType::SizeType   size;
  RGBImageType::IndexType  start;

  size[0] = 127;
  size[1] = 127;

  start[0] = 0;
  start[1] = 0;

  region.SetIndex(start);
  region.SetSize(size);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIteratorWithIndex< RGBImageType > iterator( image, image->GetLargestPossibleRegion() );
  iterator.GoToBegin();

  RGBPixelType pixel;
  RGBImageType::IndexType index;

  while(!iterator.IsAtEnd())
    {
    index = iterator.GetIndex();
    if(index[0] < 70 && index[0] > 50 && index[1] > 50 && index[1] < 70)
      {
      pixel.SetRed(255);
      pixel.SetGreen(0);
      pixel.SetBlue(0);
      iterator.Set(pixel);
      }
    else if(index[0] < 110 && index[0] > 100 && index[1] > 100 && index[1] < 110)
      {
      pixel.SetRed(0);
      pixel.SetGreen(255);
      pixel.SetBlue(0);
      iterator.Set(pixel);
      }
    else
      {
      pixel.SetRed(0);
      pixel.SetGreen(0);
      pixel.SetBlue(0);
      iterator.Set(pixel);
      }

    ++iterator;
    }
}
