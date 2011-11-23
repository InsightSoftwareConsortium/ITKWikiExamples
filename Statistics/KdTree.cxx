#include "itkVector.h"
#include "itkListSample.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkEuclideanDistanceMetric.h"

int main()
{
  typedef itk::Vector< float, 2 > MeasurementVectorType;

  typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType;
  SampleType::Pointer sample = SampleType::New();
  sample->SetMeasurementVectorSize( 2 );

  MeasurementVectorType mv;
  for (unsigned int i = 0 ; i < 100 ; ++i )
    {
    mv[0] = static_cast<float>(i);
    mv[1] = static_cast<float>(i);
    sample->PushBack( mv );
    }

  typedef itk::Statistics::KdTreeGenerator< SampleType > TreeGeneratorType;
  TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();
  treeGenerator->SetSample( sample );
  treeGenerator->SetBucketSize( 16 );
  treeGenerator->Update();

  typedef TreeGeneratorType::KdTreeType TreeType;
  typedef TreeType::NearestNeighbors NeighborsType;
  typedef TreeType::KdTreeNodeType NodeType;

  TreeType::Pointer tree = treeGenerator->GetOutput();

  MeasurementVectorType queryPoint;
  queryPoint[0] = 10.0;
  queryPoint[1] = 7.0;

  // K-Neighbor search
  std::cout << "K-Neighbor search:" << std::endl;
  unsigned int numberOfNeighbors = 3;
  TreeType::InstanceIdentifierVectorType neighbors;
  tree->Search( queryPoint, numberOfNeighbors, neighbors ) ;

  for ( unsigned int i = 0 ; i < neighbors.size() ; ++i )
    {
    std::cout << tree->GetMeasurementVector( neighbors[i] ) << std::endl;
    }
    
  // Radius search
  std::cout << "Radius search:" << std::endl;
  double radius = 4.0;
  tree->Search( queryPoint, radius, neighbors ) ;
  std::cout << "There are " << neighbors.size() << " neighbors." << std::endl;
  for ( unsigned int i = 0 ; i < neighbors.size() ; ++i )
    {
    std::cout << tree->GetMeasurementVector( neighbors[i] ) << std::endl;
    }

  return EXIT_SUCCESS;
}
