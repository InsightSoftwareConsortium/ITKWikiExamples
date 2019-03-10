#include "itkSpatialObjectToImageFilter.h"
#include "itkPlaneSpatialObject.h"
#include "itkImageFileWriter.h"

int main( int itkNotUsed( argc ), char * itkNotUsed( argv )[] )
{
  constexpr unsigned int Dimension = 2;

  using PlaneType = itk::PlaneSpatialObject< Dimension >;

  // Create a plane
  PlaneType::Pointer plane = PlaneType::New();

  return EXIT_SUCCESS;
}
