#include <itkImageRandomNonRepeatingConstIteratorWithIndex.h>

int main(int, char*[])
{
  itk::RandomPermutation rp(5);

  std::cout << std::endl;

  for(unsigned int i = 0; i < 5; i++)
    {
    std::cout << rp[i] << " ";
    }
  std::cout << std::endl << std::endl;
  
  rp.Shuffle();
  std::cout << "After shuffle" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
    {
    std::cout << rp[i] << " ";
    }
  std::cout << std::endl;

  
  return EXIT_SUCCESS;
}
