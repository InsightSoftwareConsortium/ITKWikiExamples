#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "ImageFilterMultipleOutputsDifferentType.h"

int main(int, char*[])
{
  // Setup types
  typedef itk::Image<unsigned char, 2>   InputImageType;
  typedef itk::Image<float, 2>   OutputImageType1;
  typedef itk::Image<int, 2>   OutputImageType2;
  typedef itk::ImageFilterMultipleOutputsDifferentType<InputImageType, OutputImageType1, OutputImageType2>  FilterType;

  // Create and the filter
  FilterType::Pointer filter = FilterType::New();
  filter->Update();

  {
  typedef  itk::ImageFileWriter< OutputImageType1  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("TestOutput1.jpg");
  writer->SetInput(filter->GetOutput1());
  writer->Update();
  }
  
  {
  typedef  itk::ImageFileWriter< OutputImageType2  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("TestOutput2.jpg");
  writer->SetInput(filter->GetOutput2());
  writer->Update();
  }
  
  return EXIT_SUCCESS;
}
