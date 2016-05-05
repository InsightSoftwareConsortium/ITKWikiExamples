#include <itkImage.h>

#include <iostream>

typedef itk::Image<float, 2> ImageType;
static void myStandardPointer(const ImageType*)
{
}

static void mySmartPointer(const ImageType::Pointer)
{
}

template <typename TImage>
static void TemplateSmartPointer(const typename TImage::Pointer)
{
}

template <typename TImage>
static void TemplateStandardPointer(const TImage*)
{
}

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{10,10}};
  itk::ImageRegion<2> region(corner,size);
  image->SetRegions(region);
  image->Allocate();

  // A function that accepts a standard pointer can be passed either a standard or a smart pointer
  myStandardPointer(image.GetPointer());
  myStandardPointer(image);

  // A function that accepts a smart pointer can be passed either a standard or a smart pointer
  mySmartPointer(image.GetPointer());
  mySmartPointer(image);

  return 0;
}
