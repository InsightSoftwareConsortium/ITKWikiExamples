#include "itkIndex.h"
#include <map>

int main(int, char *[])
{
  using IndexType = itk::Index<2>;
  IndexType index = {{ 3, 4 }};
  std::map<IndexType, float> myMap;
  myMap[index] = 42;
  return EXIT_SUCCESS;
}
