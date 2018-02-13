#include <itkImage.h>

int main(int, char*[])
{
  using ImageType = itk::Image<unsigned char, 2>;
  ImageType::Pointer image = ImageType::New();

  std::cout << "image is type: " << image->GetNameOfClass() << std::endl;

  return EXIT_SUCCESS;
}
