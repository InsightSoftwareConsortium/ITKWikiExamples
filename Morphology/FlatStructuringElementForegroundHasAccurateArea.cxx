#include "itkFlatStructuringElement.h"

// Helper function
template< class SEType>
bool ComputeAreaError(SEType k, unsigned int thickness = 0);

int main(int, char *[])
{
  int scalarRadius = 5;
  int scalarThickness = 2;
  bool foregroundHasAccurateArea = true;

  typedef itk::FlatStructuringElement< 2 > SE2Type;
  SE2Type::RadiusType r2;
  r2.Fill( scalarRadius );
  SE2Type k2;

  std::cout << "2D ball of radius " << scalarRadius
  << " with foregroundHasAccurateArea mode off:" << std::endl;
  k2 = SE2Type::Ball( r2 );
  ComputeAreaError(k2);

  // Test the foregroundHasAccurateArea mode.
  std::cout << "2D ball of radius " << scalarRadius
  << " with foregroundHasAccurateArea mode on:" << std::endl;
  k2 = SE2Type::Ball(r2, foregroundHasAccurateArea);
  ComputeAreaError(k2);

  std::cout << "2D annulus of radius " << scalarRadius
  << " and thickness " <<  scalarThickness
  << " with foregroundHasAccurateArea mode off:" << std::endl;
  k2 = SE2Type::Annulus(r2,scalarThickness,false);
  ComputeAreaError(k2,scalarThickness);

  // Test the foregroundHasAccurateArea mode.
  std::cout << "2D annulus of radius " << scalarRadius
  << " and thickness " <<  scalarThickness
  << " with foregroundHasAccurateArea mode on:" << std::endl;
  k2 = SE2Type::Annulus(r2,scalarThickness,false,foregroundHasAccurateArea);
  ComputeAreaError(k2,scalarThickness);

  typedef itk::FlatStructuringElement< 3 > SE3Type;
  SE3Type::RadiusType r3;
  r3.Fill( scalarRadius );
  SE3Type k3;

  std::cout << "3D ball of radius " << scalarRadius
  << " with foregroundHasAccurateArea mode off:" << std::endl;
  k3 = SE3Type::Ball( r3 );
  ComputeAreaError(k3);

  // Test the foregroundHasAccurateArea mode.
  std::cout << "3D ball of radius " << scalarRadius
  << " with foregroundHasAccurateArea mode on:" << std::endl;
  k3 = SE3Type::Ball(r3, foregroundHasAccurateArea);
  ComputeAreaError(k3);

  std::cout << "3D annulus of radius " << scalarRadius
  << " and thickness " <<  scalarThickness
  << " with foregroundHasAccurateArea mode off:" << std::endl;
  k3 = SE3Type::Annulus(r3,scalarThickness,false);
  ComputeAreaError(k3,scalarThickness);

  // Test the foregroundHasAccurateArea mode.
  std::cout << "3D annulus of radius " << scalarRadius
  << " and thickness " <<  scalarThickness
  << " with foregroundHasAccurateArea mode on:" << std::endl;
  k3 = SE3Type::Annulus(r3,scalarThickness,false,foregroundHasAccurateArea);
  ComputeAreaError(k3,scalarThickness);

  typedef itk::FlatStructuringElement< 4 > SE4Type;
  SE4Type::RadiusType r4;
  r4.Fill( scalarRadius );
  SE4Type k4;

  std::cout << "4D ball of radius " << scalarRadius
  << " with foregroundHasAccurateArea mode off:" << std::endl;
  k4 = SE4Type::Ball( r4 );
  ComputeAreaError(k4);

  // Test the foregroundHasAccurateArea mode.
  std::cout << "4D ball of radius " << scalarRadius
  << " with foregroundHasAccurateArea mode on:" << std::endl;
  k4 = SE4Type::Ball(r4, foregroundHasAccurateArea);
  ComputeAreaError(k4);

  std::cout << "4D annulus of radius " << scalarRadius
  << " and thickness " <<  scalarThickness
  << " with foregroundHasAccurateArea mode off:" << std::endl;
  k4 = SE4Type::Annulus(r4,scalarThickness,false);
  ComputeAreaError(k4,scalarThickness);

  // Test the foregroundHasAccurateArea mode.
  std::cout << "4D annulus of radius " << scalarRadius
  << " and thickness " <<  scalarThickness
  << " with foregroundHasAccurateArea mode on:" << std::endl;
  k4 = SE4Type::Annulus(r4,scalarThickness,false,foregroundHasAccurateArea);
  ComputeAreaError(k4,scalarThickness);

  return EXIT_SUCCESS;
}

template< class SEType >
bool ComputeAreaError(SEType k, unsigned int thickness)
{
  float expectedOuterForegroundArea = 1;
  float expectedInnerForegroundArea;
  if( thickness == 0 )
  {
    // Circle/Ellipse has no inner area to subract.
    expectedInnerForegroundArea = 0;
  }
  else
  {
    // Annulus does have inner area to subract.
    expectedInnerForegroundArea = 1;
  }
  if( SEType::NeighborhoodDimension == 2)
  {
    expectedOuterForegroundArea *= vnl_math::pi;
    expectedInnerForegroundArea *= vnl_math::pi;
  }
  else if( SEType::NeighborhoodDimension == 3 )
  {
    expectedOuterForegroundArea *= 4.0/3.0 * vnl_math::pi;
    expectedInnerForegroundArea *= 4.0/3.0 * vnl_math::pi;
  }
  else if ( SEType::NeighborhoodDimension == 4 )
  {
    expectedOuterForegroundArea *= 0.5 * vnl_math::pi * vnl_math::pi;
    expectedInnerForegroundArea *= 0.5 * vnl_math::pi * vnl_math::pi;
  }
  else
  {
    return EXIT_FAILURE;
  }
  for( unsigned int i = 0; i < SEType::NeighborhoodDimension; i++ )
  {
    expectedOuterForegroundArea *= k.GetRadius()[i];
    expectedInnerForegroundArea *= (k.GetRadius()[i] - thickness);
  }

  float expectedForegroundArea = expectedOuterForegroundArea - expectedInnerForegroundArea;

  // Show the neighborhood if it is 2D.
  typename SEType::Iterator SEIt;
  if( SEType::NeighborhoodDimension == 2 )
  {
    for( SEIt = k.Begin(); SEIt != k.End(); ++SEIt )
    {
      std::cout << *SEIt << "\t";
      if( (SEIt - k.Begin()+1) % k.GetSize()[0] == 0 )
      {
        std::cout << std::endl;
      }
    }
  }

  // Compute the area/volume.
  float computedForegroundArea = 0;
  for( SEIt = k.Begin(); SEIt != k.End(); ++SEIt )
  {
    if( *SEIt )
    {
      computedForegroundArea++;
    }
  }

  std::cout << "Expected foreground area: " << expectedForegroundArea << std::endl;
  std::cout << "Computed foreground area: " << computedForegroundArea << std::endl;
  std::cout << "Foreground area error: "
  << 100 * vnl_math_abs(expectedForegroundArea-computedForegroundArea)/expectedForegroundArea
  << "%" << "\n\n";
  
  return EXIT_FAILURE;
}
