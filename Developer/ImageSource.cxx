#include "itkImage.h"
#include "itkImageFileWriter.h"

#include "MyImageSource.h"

int main(int, char*[])
{
  // Setup types
  typedef itk::Image<unsigned char, 2>   ImageType;
  typedef itk::MyImageSource<ImageType>  FilterType;

  // Create and the filter
  FilterType::Pointer filter = FilterType::New();
  filter->Update();
  filter->GetOutput()->Print(std::cout);

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("Test.jpg");
  writer->SetInput(filter->GetOutput());
  writer->Update();

  
  return EXIT_SUCCESS;
}
