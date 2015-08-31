#include <itkConceptChecking.h>
#include <itkImage.h>

template <typename TImage>
void MyFunction(const TImage* const image)
{
  itkConceptMacro( nameOfCheck, ( itk::Concept::IsFloatingPoint<typename TImage::ValueType> ) );
}

int main(int, char*[])
{
  typedef itk::Image<float, 2> FloatImageType;
  FloatImageType::Pointer floatImage = FloatImageType::New();
  MyFunction(floatImage.GetPointer());

  typedef itk::Image<double, 2> DoubleImageType;
  DoubleImageType::Pointer doubleImage = DoubleImageType::New();
  MyFunction(doubleImage.GetPointer());

  // Fails the concept check
//   typedef itk::Image<int, 2> IntImageType;
//   IntImageType::Pointer intImage = IntImageType::New();
//   MyFunction(intImage.GetPointer());

  // Fails the concept check
//   typedef itk::Image<unsigned char, 2> UCharImageType;
//   UCharImageType::Pointer ucharImage = UCharImageType::New();
//   MyFunction(ucharImage.GetPointer());

  return EXIT_SUCCESS;
}
