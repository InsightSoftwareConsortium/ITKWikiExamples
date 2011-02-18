#include "itkPoint.h"
#include "itkIndex.h"

#include <iostream>
#include <string>

int main(int, char *[])
{
  itk::Index<2> pixel1;
  pixel1.Fill(2);

  itk::Index<2> pixel2;
  pixel2.Fill(4);

  itk::Point<double,2> p1;
  p1[0] = pixel1[0];
  p1[1] = pixel1[1];

  itk::Point<double,2> p2;
  p2[0] = pixel2[0];
  p2[1] = pixel2[1];

  double distance = p2.EuclideanDistanceTo(p1);
  std::cout << "Distance: " << distance << std::endl;

  return EXIT_SUCCESS;
}
