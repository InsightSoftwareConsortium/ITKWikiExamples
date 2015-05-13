#include "itkMemoryProbe.h"

#include <iostream>

static void BigFunction();

int main(int, char *[])
{
  itk::MemoryProbe memoryProbe;

  // Expect zeros here
  std::cout << "Mean: " << memoryProbe.GetMean() << std::endl;
  std::cout << "Total: " << memoryProbe.GetTotal() << std::endl;

  memoryProbe.Start();
  double* a = new double[30000];
  a[0] = 1; // Ensure the unused variable is not optimized out
  memoryProbe.Stop();

  std::cout << "Mean: " << memoryProbe.GetMean() << std::endl;
  std::cout << "Total: " << memoryProbe.GetTotal() << std::endl;

  return EXIT_SUCCESS;
}
