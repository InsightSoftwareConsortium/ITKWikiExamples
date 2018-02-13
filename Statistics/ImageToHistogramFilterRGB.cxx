#include "itkRGBPixel.h"
#include "itkImage.h"
#include "itkImageToHistogramFilter.h"
#include "itkImageRandomIteratorWithIndex.h"

using RGBPixelType = itk::RGBPixel<unsigned char>;
using ImageType = itk::Image< RGBPixelType, 2>;

void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  constexpr unsigned int MeasurementVectorSize = 3; // RGB
  constexpr unsigned int binsPerDimension = 4;
  
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  
  using ImageToHistogramFilterType = itk::Statistics::ImageToHistogramFilter< ImageType >;
  
  ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(MeasurementVectorSize);
  lowerBound.Fill(0);
  
  ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(MeasurementVectorSize);
  upperBound.Fill(255);
  
  ImageToHistogramFilterType::HistogramType::SizeType size(MeasurementVectorSize);
  size.Fill(binsPerDimension);
  
  ImageToHistogramFilterType::Pointer imageToHistogramFilter = ImageToHistogramFilterType::New();
  imageToHistogramFilter->SetInput(image);
  imageToHistogramFilter->SetHistogramBinMinimum(lowerBound);
  imageToHistogramFilter->SetHistogramBinMaximum(upperBound);
  imageToHistogramFilter->SetHistogramSize(size);
  imageToHistogramFilter->SetAutoMinimumMaximum(false);
  imageToHistogramFilter->Update();

  ImageToHistogramFilterType::HistogramType* histogram = imageToHistogramFilter->GetOutput();
  
  unsigned int totalBins = 1;
  for(unsigned int i = 0; i < size.GetNumberOfElements(); ++i)
    {
    totalBins *= histogram->GetSize()[i];
    }
  std::cout << "totalBins: " << totalBins << std::endl;
  
  std::cout << "Frequency = ";
  for(unsigned int i = 0; i < totalBins; ++i)
    {
    std::cout << histogram->GetFrequency(i) << " ";
    }
    
  std::cout << std::endl;
  
  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create a black image with a red square and a green square.
  // This should produce a histogram with very strong spikes.
  itk::Size<2> size;
  size.Fill(10);

  itk::Index<2> start;
  start.Fill(0);

  itk::ImageRegion<2> region(start, size);

  image->SetRegions(region);
  image->Allocate();
  
  RGBPixelType zeroPixel;
  zeroPixel.SetRed(0);
  zeroPixel.SetGreen(0);
  zeroPixel.SetBlue(0);
  
  image->FillBuffer(zeroPixel);
  
  RGBPixelType pixel;
  pixel.SetRed(110);
  pixel.SetGreen(110);
  pixel.SetBlue(110);
  
  itk::ImageRandomIteratorWithIndex<ImageType> imageIterator(image, image->GetLargestPossibleRegion());
  imageIterator.SetNumberOfSamples(10);
 
  while(!imageIterator.IsAtEnd())
    {
    std::cout << "Setting pixel " << imageIterator.GetIndex() << " to " << pixel << std::endl;
    imageIterator.Set(pixel);
    ++imageIterator;
    }

}
