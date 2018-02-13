#include "itkSpatialObjectToImageFilter.h"
#include "itkPlaneSpatialObject.h"
#include "itkImageFileWriter.h"

int main( int argc, char *argv[] )
{
  constexpr unsigned int Dimension = 2;

  using PlaneType = itk::PlaneSpatialObject< Dimension >;

  // Create a plane
  PlaneType::Pointer plane = PlaneType::New();

  return EXIT_SUCCESS;
}
