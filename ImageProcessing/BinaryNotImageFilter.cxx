#include "itkImage.h"
#if ITK_VERSION_MAJOR >= 4
#include "itkBinaryNotImageFilter.h"
#endif
#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"

using ImageType = itk::Image<unsigned char, 2>;
static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
#if ITK_VERSION_MAJOR >= 4
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  using WriterType = itk::ImageFileWriter< ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("input.png");
  writer->SetInput(image);
  writer->Update();
  
  using BinaryNotImageFilterType = itk::BinaryNotImageFilter <ImageType>;

  BinaryNotImageFilterType::Pointer binaryNotFilter
          = BinaryNotImageFilterType::New();
  binaryNotFilter->SetInput(image);
  binaryNotFilter->Update();

  writer->SetFileName("output.png");
  writer->SetInput(binaryNotFilter->GetOutput());
  writer->Update();
#endif
  
  return EXIT_SUCCESS;
}
void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(100);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > 50)
      {
      imageIterator.Set(255);
      }
    else
      {
      imageIterator.Set(0);
      }

    ++imageIterator;
    }

}
