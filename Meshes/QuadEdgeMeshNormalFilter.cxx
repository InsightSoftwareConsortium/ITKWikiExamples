#include "itkVector.h"
#include "itkQuadEdgeMesh.h"
#include "itkVTKPolyDataReader.h"

#include "itkQuadEdgeMeshExtendedTraits.h"
#include "itkNormalQuadEdgeMeshFilter.h"
#include <cstdlib>

int main( int argc, char* argv[] )
{
  if( argc < 2 )
    {
    std::cout <<"This program requires at least 1 argument" <<std::endl;
    std::cout <<"1- Input filename" <<std::endl;
    std::cout <<"2- Weight type" <<std::endl;
    std::cout <<"   * 0:  GOURAUD" <<std::endl;
    std::cout <<"   * 1:  THURMER" <<std::endl;
    std::cout <<"   * 2:  AREA" <<std::endl;
    return EXIT_FAILURE;
    }

  constexpr unsigned int Dimension = 3;
  using CoordType = double;

  using InputMeshType = itk::QuadEdgeMesh< CoordType, Dimension >;

  using VectorType = itk::Vector< CoordType, Dimension >;

  using Traits = itk::QuadEdgeMeshExtendedTraits <
    VectorType,
    Dimension,
    2,
    CoordType,
    CoordType,
    VectorType,
    bool,
    bool >;

  using OutputMeshType = itk::QuadEdgeMesh < VectorType, Dimension, Traits >;

  using ReaderType = itk::VTKPolyDataReader< InputMeshType >;
  using NormalFilterType = itk::NormalQuadEdgeMeshFilter< InputMeshType, OutputMeshType >;
  NormalFilterType::WeightType weight_type;

  int param = std::stoi( argv[2] );

  if( ( param < 0 ) || ( param > 2 ) )
    {
    std::cout <<"Weight type must be either: " <<std::endl;
    std::cout <<"   * 0:  GOURAUD" <<std::endl;
    std::cout <<"   * 1:  THURMER" <<std::endl;
    std::cout <<"   * 2:  AREA" <<std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    switch( param )
      {
      default:
      case 0:
        weight_type = NormalFilterType::GOURAUD;
        break;
      case 1:
        weight_type = NormalFilterType::THURMER;
        break;
      case 2:
        weight_type = NormalFilterType::AREA;
        break;
      }
    }

  ReaderType::Pointer reader = ReaderType::New( );
  reader->SetFileName( argv[1] );

  try
    {
    reader->Update( );
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown while reading the input file " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  InputMeshType::Pointer mesh = reader->GetOutput( );

  NormalFilterType::Pointer normals = NormalFilterType::New( );
  normals->SetInput( mesh );
  normals->SetWeight( weight_type );

  try
    {
    normals->Update( );
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  OutputMeshType::Pointer output = normals->GetOutput( );

  std::cout << normals << std::endl;

  return EXIT_SUCCESS;
}
