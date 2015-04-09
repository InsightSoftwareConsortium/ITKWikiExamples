#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "ImageFilterMultipleInputs.h"

int main(int, char*[])
{
  // Setup types
  typedef itk::Image<unsigned char, 2>   ImageType;
  typedef itk::ImageFilterMultipleInputs<ImageType>  FilterType;

  ImageType::Pointer image = ImageType::New();
  ImageType::Pointer mask = ImageType::New();

  // Create and the filter
  FilterType::Pointer filter = FilterType::New();
  filter->SetInputImage(image);
  filter->SetInputMask(mask);
  filter->Update();

  return EXIT_SUCCESS;
}
