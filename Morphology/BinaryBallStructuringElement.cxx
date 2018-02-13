#include "itkImage.h"
#include <itkBinaryBallStructuringElement.h>

int main(int, char *[])
{
  using StructuringElementType = itk::BinaryBallStructuringElement<float, 2>;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(5);
  structuringElement.CreateStructuringElement();

  return EXIT_SUCCESS;
}
