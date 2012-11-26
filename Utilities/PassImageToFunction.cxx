#include <itkImage.h>

#include <iostream>

typedef itk::Image<float, 2> ImageType;

static void StandardPointer(const ImageType* input)
{

}

static void SmartPointer(const ImageType::Pointer input)
{

}

template <typename TImage>
static void TemplateSmartPointer(const typename TImage::Pointer input)
{

}

template <typename TImage>
static void TemplateStandardPointer(const TImage* input)
{

}

int main()
{
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{10,10}};
  itk::ImageRegion<2> region(corner,size);
  image->SetRegions(region);
  image->Allocate();

  // A function that accepts a standard pointer can be passed either a standard or a smart pointer
  StandardPointer(image.GetPointer());
  StandardPointer(image);

  // A function that accepts a smart pointer can be passed either a standard or a smart pointer
  SmartPointer(image.GetPointer());
  SmartPointer(image);

  // TemplateSmartPointer(image); // This does not work
  //TemplateSmartPointer(image.GetPointer()); // This does not work
  TemplateSmartPointer<ImageType>(image);

  //TemplateStandardPointer(image); // This does not work
  TemplateStandardPointer<ImageType>(image);
  TemplateStandardPointer(image.GetPointer());
  
  return 0;
}
