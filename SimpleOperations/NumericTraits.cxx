#include "itkNumericTraits.h"

int main(int, char* [] )
{
  std::cout << "Min: " << itk::NumericTraits< float >::min() << std::endl;
  std::cout << "Max: " << itk::NumericTraits< float >::max() << std::endl;
  std::cout << "Zero: " << itk::NumericTraits< float >::Zero << std::endl;
  std::cout << "Zero: " << itk::NumericTraits< float >::ZeroValue() << std::endl;
  std::cout << "Is -1 negative? " << itk::NumericTraits< float >::IsNegative(-1) << std::endl;
  std::cout << "Is 1 negative? " << itk::NumericTraits< float >::IsNegative(1) << std::endl;
  std::cout << "One: " << itk::NumericTraits< float >::One << std::endl;
  std::cout << "Epsilon: " << itk::NumericTraits< float >::epsilon() << std::endl;
  std::cout << "Infinity: " << itk::NumericTraits< float >::infinity() << std::endl;

  if(0 == itk::NumericTraits< float >::infinity())
    {
    std::cout << " 0 == inf!" << std::endl;
    }
  else
    {
    std::cout << "Good" << std::endl;
    }
    
  return EXIT_SUCCESS;
}
