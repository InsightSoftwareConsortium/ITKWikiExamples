#include <itkMatrix.h>
#include <itkVector.h>

#include <iostream>

static void Construct();
//static void ConstructRunTimeDims();
static void Multiply();
//static void Inverse();

int main(int, char *[])
{
  Construct();
  Multiply();
  return EXIT_SUCCESS;
}

void Construct()
{
  using MatrixType = itk::Matrix<double, 3, 3>;
  MatrixType M;
  M(0,0) = 1.0;
  M(0,1) = 2.0;
  M(0,2) = 3.0;
  M(1,0) = 4.0;
  M(1,1) = 5.0;
  M(1,2) = 6.0;
  M(2,0) = 7.0;
  M(2,1) = 8.0;
  M(2,2) = 9.0;

  std::cout << "M: " << M << std::endl;
}

/*
void ConstructRunTimeDims()
{
  int matrixSize = 3;
  using MatrixType = itk::Matrix<double, matrixSize, matrixSize>;
  MatrixType M;
  M(0,0) = 1.0;
  M(0,1) = 2.0;
  M(0,2) = 3.0;
  M(1,0) = 4.0;
  M(1,1) = 5.0;
  M(1,2) = 6.0;
  M(2,0) = 7.0;
  M(2,1) = 8.0;
  M(2,2) = 9.0;

  std::cout << "M: " << M << std::endl;
}
*/

void Multiply()
{
  using MatrixType = itk::Matrix<double, 3, 3>;
  MatrixType M;
  M(0,0) = 1.0;
  M(0,1) = 2.0;
  M(0,2) = 3.0;
  M(1,0) = 4.0;
  M(1,1) = 5.0;
  M(1,2) = 6.0;
  M(2,0) = 7.0;
  M(2,1) = 8.0;
  M(2,2) = 9.0;

  std::cout << "M: " << M << std::endl;

  using VectorType = itk::Vector<double, 3>;
  VectorType V;
  V[0] = 1.0;
  V[1] = 2.0;
  V[2] = 3.0;

  std::cout << "V: " << V << std::endl;

  std::cout << "MV: " << M*V << std::endl;
}

/*
void Inverse()
{

}
*/
