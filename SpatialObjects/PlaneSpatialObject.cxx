#include "itkSpatialObjectToImageFilter.h"
#include "itkPlaneSpatialObject.h"
#include "itkImageFileWriter.h"

int main( int argc, char *argv[] )
{
  const unsigned int Dimension = 2;

  typedef itk::PlaneSpatialObject< Dimension >   PlaneType;

  // Create a plane
  PlaneType::Pointer plane = PlaneType::New();

  return EXIT_SUCCESS;
}
