#include "itkVTKPolyDataReader.h"

#include "itkMesh.h"
#include "itkPointSet.h"

#include <string>
#include <iostream>

int main( int argc, char *argv[] )
{
  if (argc < 2)
    {
    std::cout << "Usage: " << argv[0]
              << " Input(.vtk)" << std::endl;
    return EXIT_FAILURE;
    }

  std::string InputFilename = argv[1];
  std::cout << "Input file: " << InputFilename << std::endl;
  
  //using PointSetType = itk::PointSet<double, 3 >;
  //PointSetType::Pointer pointsSet = PointSetType::New();
  //using PointType = PointSetType::PointType;
  
  using MeshType = itk::Mesh<float, 3>;
  using ReaderType = itk::VTKPolyDataReader< MeshType >;

  ReaderType::Pointer  polyDataReader = ReaderType::New();

  using PointType = ReaderType::PointType;

  polyDataReader->SetFileName(InputFilename.c_str());
  
  try
    {
    polyDataReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Error during Update() " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  //polyDataReader->Update();
  
  std::cout << "polyDataReader:" << std::endl;
  std::cout << polyDataReader << std::endl;

  MeshType::Pointer mesh = polyDataReader->GetOutput();

  std::cout << "Testing itk::VTKPolyDataReader" << std::endl;

  unsigned int numberOfPoints = mesh->GetNumberOfPoints();
  unsigned int numberOfCells  = mesh->GetNumberOfCells();

  std::cout << "numberOfPoints= " << numberOfPoints << std::endl;
  std::cout << "numberOfCells= " << numberOfCells << std::endl;

  if( !numberOfPoints )
    {
    std::cerr << "ERROR: numberOfPoints= " << numberOfPoints << std::endl;
    return EXIT_FAILURE;
    }

  if( !numberOfCells )
    {
    std::cerr << "ERROR: numberOfCells= " << numberOfCells << std::endl;
    return EXIT_FAILURE;
    }

  // Retrieve points
  for(unsigned int i = 0; i < numberOfPoints; i++)
    {
    PointType pp;
    bool pointExists = mesh->GetPoint(i, &pp);

    if(pointExists) 
      {
      std::cout << "Point is = " << pp << std::endl;
      }
    }
  return EXIT_SUCCESS;
}
