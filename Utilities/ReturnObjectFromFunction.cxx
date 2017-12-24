#include "itkImage.h"

namespace
{
typedef itk::Image<unsigned char, 2> ImageType;
}

ImageType::Pointer ReturnSmartPointer()
{
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{10,10}};
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();

  return image;
}

ImageType* ReturnPointer()
{
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{10,10}};
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();

  return image;
}

int main(int, char *[])
{
  {
  // This is how it should be done.
  ImageType::Pointer smartPointer = ReturnSmartPointer();
  std::cout << smartPointer->GetLargestPossibleRegion() << std::endl;
  }

  {
  ImageType* pointer = ReturnPointer();
  // This crashes the program because the smart pointer created in the function goes out of scope and gets deleted
  // because it is returned as a normal pointer.
  // std::cout << pointer->GetLargestPossibleRegion() << std::endl;
  pointer = nullptr; // Here to silence warning
  }

  {
  ImageType* pointer = ReturnSmartPointer();
  // This crashes the program because though the function returned a ::Pointer, it was not stored
  // anywhere so the reference count was not increased, so it got deleted.
  // std::cout << pointer->GetLargestPossibleRegion() << std::endl;
  pointer = nullptr; // Here to silence warning
  }

  {
  // I thought this might also work, but it does not (crash).
  // My reasoning was that even though you don't return a smart pointer, you assign the object to a smart
  // pointer at return time, so it still has a reference count of 1.
  //ImageType::Pointer smartPointer = ReturnPointer(); // this line causes a 'glibc error'
  // std::cout << smartPointer->GetLargestPossibleRegion() << std::endl;
  }

  return 0;
}
