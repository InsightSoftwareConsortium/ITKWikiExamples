#include "itkImage.h"
#include "itkImageFileWriter.h"

#include "MyImageSource.h"

int main(int, char*[])
{
  // Setup types
  using ImageType = itk::Image<unsigned char, 2>;
  using FilterType = itk::MyImageSource<ImageType>;

  // Create and the filter
  FilterType::Pointer filter = FilterType::New();
  filter->Update();
  filter->GetOutput()->Print(std::cout);

  using WriterType = itk::ImageFileWriter< ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("Test.jpg");
  writer->SetInput(filter->GetOutput());
  writer->Update();

  
  return EXIT_SUCCESS;
}
