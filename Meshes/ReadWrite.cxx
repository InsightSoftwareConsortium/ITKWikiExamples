// Include the relevant header files
#include "itkMesh.h"
#include "itkRegularSphereMeshSource.h"
#include "itkMeshFileWriter.h"
#include "itkMeshFileReader.h"

// Create convenience type alias
const unsigned int Dimension = 3;
using TCoordinate = float;

using TMesh = itk::Mesh< TCoordinate, Dimension >;
using TSphere = itk::RegularSphereMeshSource< TMesh >;
using TReader = itk::MeshFileReader< TMesh >;
using TWriter = itk::MeshFileWriter< TMesh >;

int main(int argc, char * argv[] )
{

  // The user can choose to provide a commandline argument
  // between 0 and 8, in order to specify the file type.
  // If no argument is provided, zero will be used.
  // If an invalid integer is provided, zero will be used.
  // If the integer is less than zero, zero will be used.
  // If the integer is greater than seven, seven will be used.
  int i = 0;
  if (argc == 2) i = atoi(argv[1]);
  i = std::max(std::min(i,8),0);

  // Create a string using the appropriate file extension.
  std::string fileName = "sphere";

  switch (i) {
    case 0: fileName += ".vtk"; break;
    case 1: fileName += ".byu"; break;
    case 2: fileName += ".fcv"; break;
    case 3: fileName += ".fsa"; break;
    case 4: fileName += ".fsb"; break;
    case 5: fileName += ".gii"; break;
    case 6: fileName += ".obj"; break;
    case 7: fileName += ".off"; break;
  }

  // Print out the file name.
  std::cout << "File Name: " << fileName << std::endl;

  // Create the sphere source, writer, and reader.
  TSphere::Pointer sphere = TSphere::New();
  TWriter::Pointer writer = TWriter::New();
  TReader::Pointer reader = TReader::New();

  // Set the writer's input and file name, then update.
  writer->SetInput( sphere->GetOutput() );
  writer->SetFileName( fileName );
  writer->Update();

  // Set the reader's file name, then update.
  reader->SetFileName( fileName );
  reader->Update();

  return EXIT_SUCCESS;

}
