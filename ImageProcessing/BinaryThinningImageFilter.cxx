#include "itkBinaryThinningImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

using ImageType = itk::Image<unsigned char, 2>;

static void WriteImage(const ImageType::Pointer image, const std::string& fileName);
static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  if( argc == 2)
    {
    using ImageReader = itk::ImageFileReader<ImageType>;
    ImageReader::Pointer reader = ImageReader::New();
    std::string fileName = argv[1];
    reader->SetFileName(fileName);
    reader->Update();
    image = reader->GetOutput();
    }
  else
    {
    CreateImage(image);
    WriteImage(image, "input.png");
    }

  using BinaryThinningImageFilterType = itk::BinaryThinningImageFilter <ImageType, ImageType>;
  BinaryThinningImageFilterType::Pointer binaryThinningImageFilter = BinaryThinningImageFilterType::New();
  binaryThinningImageFilter->SetInput(image);
  binaryThinningImageFilter->Update();

  // Rescale the image so that it can be seen (the output is 0 and 1, we want 0 and 255)
  using RescaleType = itk::RescaleIntensityImageFilter< ImageType, ImageType >;
  RescaleType::Pointer rescaler = RescaleType::New();
  rescaler->SetInput( binaryThinningImageFilter->GetOutput() );
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->Update();
  
  WriteImage(rescaler->GetOutput(), "output.png");

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create an image
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(100);

  ImageType::RegionType region(start,size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // Draw a 5 pixel wide line
  for(unsigned int i = 20; i < 80; ++i)
    {
    for(unsigned int j = 50; j < 55; ++j)
      {
      itk::Index<2> index;
      index[0] = i;
      index[1] = j;
      image->SetPixel(index, 255);
      }
    }
}

void WriteImage(const ImageType::Pointer image, const std::string& fileName)
{
  using WriterType = itk::ImageFileWriter< ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(fileName);
  writer->SetInput(image);
  writer->Update();
}
