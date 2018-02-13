#include "itkImage.h"
#include "itkAddPixelAccessor.h"
#include "itkImageAdaptor.h"
#include "itkImageRegionIterator.h"
 
using ImageType = itk::Image<unsigned int, 2>;
 
static void CreateImage(ImageType::Pointer image);
 
int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
 
  using AddPixelAccessorType = itk::Accessor::AddPixelAccessor <ImageType::PixelType>;
  using ImageAdaptorType = itk::ImageAdaptor<  ImageType, AddPixelAccessorType >;

  ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();
  AddPixelAccessorType addPixelAccessor;


  adaptor->SetImage(image);
 
  ImageType::IndexType index;
  index[0] = 0;
  index[1] = 0;

  addPixelAccessor.SetValue(5);
  adaptor->SetPixelAccessor(addPixelAccessor);
  std::cout << "addPixelAccessor.SetValue(5)" << std::endl;
  std::cout << "\timage->GetPixel" << index << ": " << image->GetPixel(index)
            << " adaptor->GetPixel" << index << ": " << adaptor->GetPixel(index)
            << std::endl;

  addPixelAccessor.SetValue(100);
  adaptor->SetPixelAccessor(addPixelAccessor);
  std::cout << "addPixelAccessor.SetValue(100)" << std::endl;
  std::cout << "\timage->GetPixel" << index << ": " << image->GetPixel(index)
            << " adaptor->GetPixel" << index << ": " << adaptor->GetPixel(index)
            << std::endl;

  return EXIT_SUCCESS;
}
 
void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);
 
  ImageType::SizeType size;
  size.Fill(10);
 
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
 
  image->SetRegions(region);
  image->Allocate();
 
  itk::ImageRegionIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());
 
  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(20);
    ++imageIterator;
    }
 
}
