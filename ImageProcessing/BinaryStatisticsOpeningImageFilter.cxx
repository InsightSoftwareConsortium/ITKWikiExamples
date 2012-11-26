#include "itkBinaryStatisticsOpeningImageFilter.h"
#include "itkImageFileWriter.h"

typedef itk::Image<unsigned char, 2> ImageType;
void CreateImage(ImageType::Pointer image1, ImageType::Pointer image2);

int main(int argc, char* argv[])
{
  ImageType::Pointer binaryImage = ImageType::New();
  ImageType::Pointer featureImage = ImageType::New();
    
  CreateImage(binaryImage, featureImage);
 
  typedef itk::BinaryStatisticsOpeningImageFilter<ImageType, ImageType> 
    BinaryOpeningType;
  BinaryOpeningType::Pointer opening = BinaryOpeningType::New();
  opening->SetInput(binaryImage);
  opening->SetFeatureImage(featureImage);
  opening->SetBackgroundValue(0);
  opening->SetForegroundValue(255);
  opening->SetLambda(150);
  opening->SetAttribute( BinaryOpeningType::LabelObjectType::MEAN);
  opening->Update();

  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("input.mhd");
  writer->SetInput(featureImage);
  writer->Update();
  writer->SetFileName("output.mhd");
  writer->SetInput(opening->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image1, ImageType::Pointer image2)
{
  // Create an image with 2 connected components
  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
 
  ImageType::SizeType size;
  size[0] = 200;
  size[1] = 200;
 
  region.SetSize(size);
  region.SetIndex(start);
 
  image1->SetRegions(region);
  image1->Allocate();
  image1->FillBuffer(0);
 
  image2->SetRegions(region);
  image2->Allocate();
  image2->FillBuffer(0);
 
  // Make a square
  for(unsigned int r = 20; r < 80; r++)
  {
    for(unsigned int c = 30; c < 100; c++)
    {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;
 
      image1->SetPixel(pixelIndex, 255);
      image2->SetPixel(pixelIndex, 100);
    }
  }
 
  // Make another square
  for(unsigned int r = 100; r < 130; r++)
  {
    for(unsigned int c = 115; c < 160; c++)
    {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;
      
      image1->SetPixel(pixelIndex, 255);
      image2->SetPixel(pixelIndex, 200);
    }
  }
}
