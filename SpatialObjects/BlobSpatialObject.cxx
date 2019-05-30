#include "itkBlobSpatialObject.h"

int main( int itkNotUsed( argc ), char * itkNotUsed( argv )[] )
{
  using BlobType = itk::BlobSpatialObject<2>;

  // Create a list of points
  BlobType::BlobPointListType points;
  for(unsigned int i = 0; i < 20; i++)
    {
    BlobType::BlobPointType point;
    point.SetPositionInObjectSpace (i,i);

    points.push_back(point);
    }

  BlobType::Pointer blob = BlobType::New();
  blob->SetPoints(points);
  
  BlobType::BoundingBoxType::BoundsArrayType bounds = blob->GetMyBoundingBoxInWorldSpace()->GetBounds();
  std::cout << "Bounds: " << bounds << std::endl;
  
  return EXIT_SUCCESS;
}
