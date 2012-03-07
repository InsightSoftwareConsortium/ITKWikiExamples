#include <itkImage.h>

#include <iostream>

typedef itk::Image<float, 2> ImageType;

static void Test(const ImageType* input, ImageType* output);

int main()
{
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size;
  size.Fill(100);

  itk::ImageRegion<2> region(start,size);

  image->SetRegions(region);
  image->Allocate();
  
  ImageType::Pointer outputImage = ImageType::New();

  Test(image, outputImage); // This seems to automatically work.
  //Test(image.GetPointer(), outputImage.GetPointer()); // This explicitly sends an ImageType*

  std::cout << outputImage->GetLargestPossibleRegion().GetSize() << std::endl;
  return 0;
}


void Test(const ImageType* input, ImageType* output)
{

  if(output->GetLargestPossibleRegion() != input->GetLargestPossibleRegion()) // Crashes here
    {
    output->SetRegions(input->GetLargestPossibleRegion());
    output->Allocate();
    }
}
