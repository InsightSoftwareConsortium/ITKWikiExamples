#include <itkImageRandomNonRepeatingConstIteratorWithIndex.h>

int main(int, char*[])
{
  itk::RandomPermutation rp(5);
#if !defined(ITK_LEGACY_REMOVE)
  rp.Dump();
#endif
  std::cout << std::endl;

  for(unsigned int i = 0; i < 5; i++)
    {
    std::cout << rp[i] << " ";
    }
  std::cout << std::endl << std::endl;
  
  rp.Shuffle();
#if !defined(ITK_LEGACY_REMOVE)
  rp.Dump();
#endif
  
  return EXIT_SUCCESS;
}
