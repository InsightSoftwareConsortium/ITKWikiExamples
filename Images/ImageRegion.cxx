#include "itkImageRegion.h"
#include "itkIndex.h"
#include "itkSize.h"

int main(int, char *[])
{
  itk::Size<2> size;
  size.Fill(3);

  itk::Index<2> index;
  index.Fill(1);
  
  typedef itk::ImageRegion<2> RegionType;
  RegionType region(index,size);

  std::cout << region << std::endl;

  return EXIT_SUCCESS;
}
