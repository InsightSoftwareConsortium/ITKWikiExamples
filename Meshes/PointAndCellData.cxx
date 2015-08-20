// Include the relevant header files.
#include "itkMesh.h"
#include "itkRegularSphereMeshSource.h"
#include "itkMeshFileWriter.h"

// We define the dimension and coordinate type...
const unsigned int Dimension = 3;
typedef float      TCoordinate;

// ...and then typedef the mesh, sphere, and writer.
typedef itk::Mesh< TCoordinate, Dimension >   TMesh;
typedef itk::RegularSphereMeshSource< TMesh > TSphere;
typedef itk::MeshFileWriter< TMesh >          TMeshWriter;
 
int main()
{

  // Create the sphere source. 
  TSphere::Pointer sphere = TSphere::New();
  sphere->Update();

  // We now assign it to a mesh pointer. 
  TMesh::Pointer mesh = sphere->GetOutput();
  
  // It is necessary to disconnect the mesh from the pipeline;
  // otherwise, the point and cell data will be deallocated
  // when we call "Update()" on the writer later in the program.
  mesh->DisconnectPipeline();

  // Let's assign a value to each of the mesh's points...
  for (unsigned int i = 0; i < mesh->GetNumberOfPoints(); ++i)
    mesh->SetPointData( i, 5.0 ); 

  // ...and assign a different value to each of the mesh's cells.
  for (unsigned int i = 0; i < mesh->GetNumberOfCells(); ++i)
    mesh->SetCellData( i, 10.0 );

  // We'll print out some data about the points...
  std::cout << mesh->GetNumberOfPoints() << std::endl; // 66
  std::cout << mesh->GetPointData()->Size() << std::endl; // 66
  std::cout << mesh->GetPointData()->ElementAt( 0 ) << std::endl << std::endl; // 5.0
 
  // ...and about the cells.
  std::cout << mesh->GetNumberOfCells() << std::endl; // 128
  std::cout << mesh->GetCellData()->Size() << std::endl; // 128
  std::cout << mesh->GetCellData()->ElementAt( 0 ) << std::endl << std::endl; // 10.0

  // Finally, we'll write the data to file.  Note that the only mesh file
  // formats supported by ITK which support cell and point data are .vtk and .gii.
  TMeshWriter::Pointer meshWriter = TMeshWriter::New();
  meshWriter->SetFileName( "mesh.vtk" );
  meshWriter->SetInput( mesh );
  meshWriter->Update();
 
  return EXIT_SUCCESS;
 
}
