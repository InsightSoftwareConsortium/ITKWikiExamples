#include "itkMovingHistogramImageFilter.h"
#include "itkFlatStructuringElement.h"

typedef itk::Image<unsigned char, 2> ImageType;
  
void CreateImage(ImageType::Pointer image);

template <typename T>
class MyHistogram
{
public:
  MyHistogram()
  {
    this->Bins.resize(256);
  }
  
  void AddPixel(T pixel)
  {
    std::cout << "AddPixel " << static_cast<int>(pixel) << std::endl;
    this->Bins[pixel]++;
    OutputCurrentHistogram();
  }
  
  void AddBoundary()
  {
    std::cout << "AddBoundary" << std::endl;
  }
  
  T GetValue(T pixel)
  {
    std::cout << "GetValue" << std::endl;
    return 0;
  }

  void RemovePixel(T pixel)
  {
    std::cout << "RemovePixel " << static_cast<int>(pixel) << std::endl;
    this->Bins[pixel]--;
    OutputCurrentHistogram();
  }

  void RemoveBoundary()
  {
    std::cout << "RemoveBoundary" << std::endl;
  }

  void OutputCurrentHistogram()
  {
    for(unsigned int i = 0; i < this->Bins.size(); ++i)
      {
      std::cout << static_cast<int>(this->Bins[i]) << " ";
      }
    std::cout << std::endl;
  }

private:
  std::vector<T> Bins;
};

int main(int argc, char*argv[])
{
  typedef itk::FlatStructuringElement<2> StructuringElementType;
  StructuringElementType::RadiusType elementRadius;
  elementRadius.Fill(3); // a 7x7 kernel
  StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);
  
  typedef MyHistogram<unsigned char> HistogramType;

  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  
  typedef itk::MovingHistogramImageFilter< ImageType, ImageType, StructuringElementType, HistogramType > MovingHistogramImageFilterType;
  MovingHistogramImageFilterType::Pointer movingHistogramImageFilter = MovingHistogramImageFilterType::New();

  movingHistogramImageFilter->SetInput(image);
  movingHistogramImageFilter->SetKernel(structuringElement);
  movingHistogramImageFilter->Update();
  
  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(10);

  ImageType::RegionType region(start, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] < 5)
      {
      imageIterator.Set(255);
      }
    ++imageIterator;
    }

}
