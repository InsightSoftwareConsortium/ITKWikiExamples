#include "itkIndex.h"

#include <map>

int main(int, char *[])
{
  // Method 1
  //itk::Index<2> index = {{ 3, 4 }};
  itk::Index<2> index;

  //std::map<itk::Index<2>, float> myMap; // This doesn't work
  std::map<itk::Index<2>, float, itk::Index<2>::LexicographicCompare> myMap;

  myMap[index] = 3;

  return EXIT_SUCCESS;
}
