#include "itkTimeProbe.h"

#include <iostream>
#include <string>

static void LongFunction();
static double unused_value=0;

int main(int, char *[])
{
  itk::TimeProbe clock;

  clock.Start();
  LongFunction();

  clock.Stop();
  std::cout << "Mean: " << clock.GetMean() << std::endl;
  std::cout << "Total: " << clock.GetTotal() << std::endl;

  clock.Start();
  LongFunction();

  clock.Stop();
  std::cout << "Mean: " << clock.GetMean() << std::endl;
  std::cout << "Total: " << clock.GetTotal() << std::endl;

  if(unused_value == -1.0) std::cout << "Never prints" << std::endl;

  return EXIT_SUCCESS;
}

void LongFunction()
{
  for(int i = 0; i < 1e8; ++i)
  {
    unused_value += i%2;
  }
}
