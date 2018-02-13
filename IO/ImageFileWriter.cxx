#include "itkImage.h"
#include "itkImageFileWriter.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  std::string outputFilename;
  if(argc > 1)
    {
    outputFilename = argv[1];
    }
  else
    {
    outputFilename = "test.png";
    }

  using PixelType = unsigned char;
  const     unsigned int    Dimension = 2;
  using ImageType = itk::Image< PixelType, Dimension >;

  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = 200;
  size[1] = 300;

  region.SetSize(size);
  region.SetIndex(start);

  ImageType::Pointer image = ImageType::New();
  image->SetRegions(region);
  image->Allocate();

  ImageType::IndexType ind;
  ind[0] = 10;
  ind[1] = 10;

  using WriterType = itk::ImageFileWriter< ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();

  return EXIT_SUCCESS;
}
