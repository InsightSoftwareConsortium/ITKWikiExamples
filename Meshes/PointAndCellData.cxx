#include "itkMesh.h"
#include "itkRegularSphereMeshSource.h"
#include "itkMeshFileWriter.h"

const unsigned int Dimension = 3;
typedef float      TCoordinate;

typedef itk::Mesh< TCoordinate, Dimension >   TMesh;
typedef itk::RegularSphereMeshSource< TMesh > TSphere;
typedef itk::MeshFileWriter< TMesh >          TMeshWriter;

int main()
{

  TSphere::Pointer sphere = TSphere::New();
  sphere->Update();

  TMesh::Pointer mesh = sphere->GetOutput();
  double value = 10;
  for (unsigned int i = 0; i < mesh->GetNumberOfCells(); ++i)
    mesh->SetCellData( i, value );

  for (unsigned int i = 0; i < mesh->GetNumberOfPoints(); ++i)
    mesh->SetPointData( i, value );

  std::cout << mesh->GetNumberOfCells() << std::endl; // 128
  std::cout << mesh->GetCellData() << std::endl; // A pointer
  std::cout << mesh->GetCellData()->Size() << std::endl << std::endl; // 128

  std::cout << mesh->GetNumberOfPoints() << std::endl; // 66
  std::cout << mesh->GetPointData() << std::endl; // Another pointer
  std::cout << mesh->GetPointData()->Size() << std::endl << std::endl; // 66

  TMeshWriter::Pointer meshWriter = TMeshWriter::New();
  meshWriter->SetFileName( "mesh.vtk" );
  meshWriter->SetInput( mesh );
  meshWriter->Update();

  std::cout << mesh->GetNumberOfCells() << std::endl; // 128
  std::cout << mesh->GetCellData() << std::endl; // Different Pointer
  std::cout << mesh->GetCellData()->Size() << std::endl << std::endl; // 0

  std::cout << mesh->GetNumberOfPoints() << std::endl; // 66
  std::cout << mesh->GetPointData() << std::endl; // Different pointer
  std::cout << mesh->GetPointData()->Size() << std::endl << std::endl; // 0

  return EXIT_SUCCESS;

}
