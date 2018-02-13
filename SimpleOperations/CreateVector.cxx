#include <itkVector.h>

int main(int, char*[])
{
  using VectorType = itk::Vector<double, 3>;
  VectorType v;
  v[0] = 1.0;
  v[1] = 2.0;
  v[2] = 3.0;
  std::cout << "v: " << v << std::endl;
  
  return EXIT_SUCCESS;
}
