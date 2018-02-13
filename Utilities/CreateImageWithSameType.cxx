#include "itkImage.h"

using FloatScalarImageType = itk::Image<float,2>;

itk::ImageBase<2>::Pointer CreateImageWithSameType(const itk::ImageBase<2>* input);
void OutputImageType(const itk::ImageBase<2>* input);

int main(int, char *[])
{
  FloatScalarImageType::Pointer floatImage = FloatScalarImageType::New();
  itk::ImageBase<2>::Pointer floatCopy = CreateImageWithSameType(floatImage);
  OutputImageType(floatCopy);
  
  return EXIT_SUCCESS;
}

itk::ImageBase<2>::Pointer CreateImageWithSameType(const itk::ImageBase<2>* input)
{
  OutputImageType(input);
  itk::LightObject::Pointer objectCopyLight = input->CreateAnother();

  itk::ImageBase<2>::Pointer objectCopy = dynamic_cast<itk::ImageBase<2>*>(objectCopyLight.GetPointer());
  OutputImageType(objectCopy);
  return objectCopy;
}

void OutputImageType(const itk::ImageBase<2>* input)
{
  if(dynamic_cast<const FloatScalarImageType*>(input))
    {
    std::cout << "Image type FloatScalarImageType" << std::endl;
    }
  else
    {
    std::cout << "Image is Invalid type!" << std::endl;
    }
}
