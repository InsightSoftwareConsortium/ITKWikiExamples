#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkContourExtractor2DImageFilter.h"
 
typedef itk::Image<unsigned char, 2>  ImageType;
 
static void CreateImage(ImageType::Pointer image);
 
int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
 
  typedef itk::ContourExtractor2DImageFilter <ImageType>
          ContourExtractor2DImageFilterType;
  ContourExtractor2DImageFilterType::Pointer contourExtractor2DImageFilter
          = ContourExtractor2DImageFilterType::New();
  contourExtractor2DImageFilter->SetInput(image);
  //contourExtractor2DImageFilter->SetContourValue(255);
  contourExtractor2DImageFilter->SetContourValue(0);
  contourExtractor2DImageFilter->Update();
 
  ContourExtractor2DImageFilterType::VertexListType::ConstIterator vertexIterator = contourExtractor2DImageFilter->GetOutput()->GetVertexList()->Begin();

  while(vertexIterator != contourExtractor2DImageFilter->GetOutput()->GetVertexList()->End())
    {
    std::cout << vertexIterator->Value() << std::endl;
    ++vertexIterator;
    }
  
  return EXIT_SUCCESS;
}
 
void CreateImage(ImageType::Pointer image)
{
  // Create an image
  ImageType::IndexType start;
  start.Fill(0);
 
  ImageType::SizeType size;
  size.Fill(100);
 
  ImageType::RegionType region(start, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // Create a line of white pixels
  for(unsigned int i = 40; i < 60; ++i)
    {
    ImageType::IndexType pixel;
    pixel.Fill(i);
    image->SetPixel(pixel, 255);
    }
    
  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("image.png");
  writer->SetInput(image);
  writer->Update();
}
