#include <itkMatrix.h>

#include <iostream>

int main()
{
  typedef itk::Matrix<double, 2, 2> MatrixType;
  MatrixType M;
  M(0,0) = 1.0;
  M(0,1) = 2.0;
  M(1,0) = 3.0;
  M(1,1) = 5.0;

  std::cout << "M: " << M << std::endl;

  MatrixType Minv( M.GetInverse() );

  std::cout << "Inverse: " << Minv << std::endl;

}
