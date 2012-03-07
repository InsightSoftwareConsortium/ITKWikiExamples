#include "itkVectorImage.h"
#include "itkImageToHistogramFilter.h"
#include "itkImageRandomIteratorWithIndex.h"

typedef itk::VectorImage< float, 2> ImageType;

const unsigned int MeasurementVectorSize = 3; // 3D vectors

void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  
  const unsigned int binsPerDimension = 4;
  
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  
  typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;
  
  ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(MeasurementVectorSize);
  lowerBound.Fill(0);
  
  std::cout << "lowerBound: " << lowerBound << std::endl;
  
  ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(MeasurementVectorSize);
  upperBound.Fill(255) ;
  
  std::cout << "upperBound: " << upperBound << std::endl;
  
  ImageToHistogramFilterType::HistogramType::SizeType size(MeasurementVectorSize);
  size.Fill(binsPerDimension);
  
  std::cout << "Size: " << size << std::endl;
  
  ImageToHistogramFilterType::Pointer imageToHistogramFilter = ImageToHistogramFilterType::New();
  imageToHistogramFilter->SetInput(image);
  imageToHistogramFilter->SetHistogramBinMinimum(lowerBound);
  imageToHistogramFilter->SetHistogramBinMaximum(upperBound);
  imageToHistogramFilter->SetHistogramSize(size);
  imageToHistogramFilter->SetAutoMinimumMaximum(false);
  imageToHistogramFilter->Update();

  ImageToHistogramFilterType::HistogramType* histogram = imageToHistogramFilter->GetOutput();

  ImageType::PixelType pixel;
  pixel.SetSize(MeasurementVectorSize);
  pixel.Fill(110);
  
  //std::cout << "id: " << histogram->GetIndex(pixel) << std::endl; // Can't do this - Image::PixelType is not the same as HistogramType::MeasurementVectorType
  
  ImageToHistogramFilterType::HistogramType::MeasurementVectorType mv(MeasurementVectorSize);
  mv[0] = pixel[0];
  mv[1] = pixel[1];
  mv[2] = pixel[2];
  std::cout << "index: " << histogram->GetIndex(mv) << std::endl;
  std::cout << "id: " << histogram->GetInstanceIdentifier(histogram->GetIndex(mv)) << std::endl;
  
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
  image->SetNumberOfComponentsPerPixel(MeasurementVectorSize);
  image->Allocate();
  
  ImageType::PixelType zeroPixel;
  zeroPixel.SetSize(MeasurementVectorSize);
  zeroPixel.Fill(0);
  
  image->FillBuffer(zeroPixel);
  
  ImageType::PixelType pixel;
  pixel.SetSize(MeasurementVectorSize);
  pixel.Fill(110);
  
  itk::ImageRandomIteratorWithIndex<ImageType> imageIterator(image, image->GetLargestPossibleRegion());
  imageIterator.SetNumberOfSamples(10);
 
  while(!imageIterator.IsAtEnd())
    {
    std::cout << "Setting pixel " << imageIterator.GetIndex() << " to " << pixel << std::endl;
    imageIterator.Set(pixel);
    ++imageIterator;
    }

}
