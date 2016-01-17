#include <itkSimplexMesh.h>
#include <itkRegularSphereMeshSource.h>
#include <itkTriangleMeshToSimplexMeshFilter.h>
#include <itkSimplexMeshVolumeCalculator.h>

typedef itk::Mesh< float, 3 >        TMesh;
typedef itk::SimplexMesh< float, 3 > TSimplex;
typedef itk::RegularSphereMeshSource< TMesh >                   TSphere;
typedef itk::TriangleMeshToSimplexMeshFilter< TMesh, TSimplex > TConvert;
typedef itk::SimplexMeshVolumeCalculator< TSimplex >            TVolume;

int main(int, char *[])
{

  // Create a spherical mesh with known radius and resolution.
  TSphere::Pointer source = TSphere::New();
  TSphere::VectorType scale;
  scale.Fill( 5.0 );
  source->SetScale( scale );
  source->SetResolution( 5 );
  source->Update();

  // Ensure that all cells of the mesh are triangles.
  for (TMesh::CellsContainerIterator it = source->GetOutput()->GetCells()->Begin();
       it != source->GetOutput()->GetCells()->End();
       ++it)
    {   
    TMesh::CellAutoPointer cell;
    source->GetOutput()->GetCell(it->Index(), cell);
    if (3 != cell->GetNumberOfPoints())
      {   
      std::cerr << "ERROR: All cells must be trianglar." << std::endl;  
      return EXIT_FAILURE;
      }   
    }   

  // Convert the triangle mesh to a simplex mesh.
  TConvert::Pointer convert = TConvert::New();
  convert->SetInput( source->GetOutput() );
  convert->Update();

  // Calculate the volume and area of the simplex mesh.
  TVolume::Pointer volume = TVolume::New();
  volume->SetSimplexMesh( convert->GetOutput() );
  volume->Compute();

  // Compare with the volume and area of an ideal sphere.
  std::cout << "Ideal Volume: "       << 4.0/3.0*M_PI*pow(5.0,3) << std::endl;
  std::cout << "Mesh Volume: "        << volume->GetVolume()     << std::endl;
  std::cout << "Ideal Surface Area: " << 4.0*M_PI*pow(5.0,2)     << std::endl;
  std::cout << "Mesh Surface Area: "  << volume->GetArea()       << std::endl;

  return EXIT_SUCCESS;

}
