#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
 
#include "itkOilPaintingImageFilter.h"
 
int main(int, char*[])
{
  typedef itk::Image<unsigned char, 2>   ImageType;
  typedef itk::OilPaintingImageFilter<ImageType>  FilterType;
 
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName("LenaGrayscale.jpg");
  reader->Update();
 
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetNumberOfBins(50);
  filter->SetRadius(2);
  filter->Update();
 
  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("LenaOil.jpg");
  writer->SetInput(filter->GetOutput());
  writer->Update();
 
  return EXIT_SUCCESS;
}
