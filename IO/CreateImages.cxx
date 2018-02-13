#include "itkImage.h"
#include "itkImageFileWriter.h"
#include <itkCovariantVector.h>

#include <string>

template<typename TImageType>
void WriteFile(typename TImageType::Pointer image, std::string filename);

int main(int, char *[])
{
  using ImageType4 = itk::Image< itk::CovariantVector<double, 4> , 2>;
  ImageType4::Pointer image4 = ImageType4::New();
  WriteFile<ImageType4>(image4, std::string("image4.mhd"));

  using ImageType5 = itk::Image< itk::CovariantVector<double, 5> , 2>;
  ImageType5::Pointer image5 = ImageType5::New();
  WriteFile<ImageType5>(image5, std::string("image5.mhd"));

  return EXIT_SUCCESS;
}

template<typename TImageType>
void WriteFile(typename TImageType::Pointer image, std::string filename)
{
  // Create image
  typename TImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  typename TImageType::SizeType size;
  size[0] = 20;
  size[1] = 30;

  typename TImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();
  
  using WriterType = typename itk::ImageFileWriter<TImageType>;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetInput(image);
  writer->SetFileName(filename);
  writer->Update();
  
}

template void WriteFile<itk::Image< itk::CovariantVector<double, 4> , 2> >(itk::Image< itk::CovariantVector<double, 4> , 2>::Pointer, std::string);
template void WriteFile<itk::Image< itk::CovariantVector<double, 5> , 2> >(itk::Image< itk::CovariantVector<double, 5> , 2>::Pointer, std::string);
