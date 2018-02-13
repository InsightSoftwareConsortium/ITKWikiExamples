#include "itkImage.h"
#include "itkSquareImageFilter.h"
#include "itkImageFileWriter.h"

using ImageType = itk::Image<unsigned char, 2>;
static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  using SquareImageFilterType = itk::SquareImageFilter <ImageType, ImageType>;
  SquareImageFilterType::Pointer squareImageFilter = SquareImageFilterType::New();
  squareImageFilter->SetInput(image);
  squareImageFilter->Update();

  using WriterType = itk::ImageFileWriter< ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("output.png");
  writer->SetInput(squareImageFilter->GetOutput());
  writer->Update();

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
    if(imageIterator.GetIndex()[0] < 70)
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
