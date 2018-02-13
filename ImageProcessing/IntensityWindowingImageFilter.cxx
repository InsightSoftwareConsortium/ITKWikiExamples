#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkIntensityWindowingImageFilter.h"
#include "itkImageRegionIterator.h"

using ImageType = itk::Image<unsigned char, 2>;

static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  using IntensityWindowingImageFilterType = itk::IntensityWindowingImageFilter <ImageType, ImageType>;

  IntensityWindowingImageFilterType::Pointer filter = IntensityWindowingImageFilterType::New();
  filter->SetInput(image);
  filter->SetWindowMinimum(0);
  filter->SetWindowMaximum(100);
  filter->SetOutputMinimum(0);
  filter->SetOutputMaximum(255);
  filter->Update();

  using WriterType = itk::ImageFileWriter< ImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("output.png");
  writer->SetInput(image);
  writer->Update();
  
  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(100);

  ImageType::RegionType region(start,size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(10);

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > 30)
      {
      imageIterator.Set(0);
      }

    ++imageIterator;
    }

}
