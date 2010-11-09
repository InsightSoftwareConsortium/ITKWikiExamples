#include "itkSize.h"

int main(int, char *[])
{
  itk::Size<2> size;

  // Method 1
  size.Fill(0);

  /*
  // Method 2
  size[0] = 1;
  size[1] = 2;
  */
  std::cout << size << std::endl;

  return EXIT_SUCCESS;
}
