#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkMedianImageFunction.h"

typedef itk::Image< unsigned char, 2 > UnsignedCharImageType;

void CreateImage(UnsignedCharImageType::Pointer image);

int main( int, char *[] )
{
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  CreateImage(image);
  
  typedef itk::MedianImageFunction< UnsignedCharImageType > MedianImageFunctionType;
  MedianImageFunctionType::Pointer medianImageFunction = MedianImageFunctionType::New();
  medianImageFunction->SetInputImage( image );
  
  itk::Index<2> index;
  index.Fill(10);
  std::cout << "Median at " << index << " is " << static_cast<int>(medianImageFunction->EvaluateAtIndex(index)) << std::endl;
  return EXIT_SUCCESS;
}

void CreateImage(UnsignedCharImageType::Pointer image)
{
  itk::Index<2> start;
  start.Fill(0);
 
  itk::Size<2> size;
  size.Fill(100);
 
  itk::ImageRegion<2> region(start,size);
 
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
 
  itk::ImageRegionIterator<UnsignedCharImageType> imageIterator(image,region);
 
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] >= 50 && imageIterator.GetIndex()[1] >= 50 &&
       imageIterator.GetIndex()[0] <= 70 && imageIterator.GetIndex()[1] <= 70)
      {
      imageIterator.Set(255);
      }
 
    ++imageIterator;
    }
 
  typedef itk::ImageFileWriter < UnsignedCharImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("input.png");
  writer->SetInput(image);
  writer->Update();
}
