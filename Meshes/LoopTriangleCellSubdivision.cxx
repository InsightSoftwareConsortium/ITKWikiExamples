#include "itkRegularSphereMeshSource.h"
#include "itkQuadEdgeMesh.h"
#include "itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h"
#include "itkIterativeTriangleCellSubdivisionQuadEdgeMeshFilter.h"
 
// Dimension and coordinate type
const unsigned int Dimension = 3;
typedef float TCoordinate;
 
// Typedefs
typedef itk::QuadEdgeMesh< TCoordinate, Dimension > TQEMesh;
typedef itk::RegularSphereMeshSource< TQEMesh > TSphere;
typedef itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter< TQEMesh, TQEMesh > TLoop;
typedef itk::IterativeTriangleCellSubdivisionQuadEdgeMeshFilter< TQEMesh, TLoop > TSubdivision;

int main(int , char * [] )
{

  // Specify the number of times the loop subdivision should be applied
  const unsigned int NumberOfSubdivisions = 2;
 
  // Create a sphere and assign it to a TQEMesh variable
  TSphere::Pointer sphereSource = TSphere::New();
  sphereSource->Update();
  TQEMesh::Pointer sphere = sphereSource->GetOutput();
 
  // Assign the sphere to the subdivision filter
  TSubdivision::Pointer subdivision = TSubdivision::New();
  subdivision->SetResolutionLevels( NumberOfSubdivisions );
  subdivision->SetInput( sphere );
  subdivision->Update();
 
  // Assign the subdivided sphere to its own variable
  TQEMesh::Pointer subSphere = subdivision->GetOutput();
 
  // Compare points, edges, and faces for the original and subdivided spheres
  std::cout << "Original Sphere:" << std::endl;
  std::cout << "  Points: " << sphere->GetNumberOfPoints() << std::endl; // 66
  std::cout << "  Edges: " << sphere->GetNumberOfEdges() << std::endl; // 192
  std::cout << "  Faces: " << sphere->GetNumberOfFaces() << std::endl; // 128
  std::cout << std::endl;
  std::cout << "Divided Sphere: " << std::endl;
  std::cout << "  Points: " << subSphere->GetNumberOfPoints() << std::endl; // 1026
  std::cout << "  Edges: " << subSphere->GetNumberOfEdges() << std::endl; // 3072
  std::cout << "  Faces: " << subSphere->GetNumberOfFaces() << std::endl; // 2048
 
  return EXIT_SUCCESS;
 
}
