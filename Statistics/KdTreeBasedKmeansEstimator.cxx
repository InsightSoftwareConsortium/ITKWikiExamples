#include "itkDecisionRule.h"
#include "itkVector.h"
#include "itkListSample.h"
#include "itkKdTree.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkKdTreeBasedKmeansEstimator.h"
#include "itkMinimumDecisionRule.h"
#include "itkEuclideanDistanceMetric.h"
#include "itkDistanceToCentroidMembershipFunction.h"
#include "itkSampleClassifierFilter.h"
#include "itkNormalVariateGenerator.h"


int main(int, char *[])
{
  typedef itk::Vector< double, 3 > MeasurementVectorType;
  typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType;
  SampleType::Pointer sample = SampleType::New();
  sample->SetMeasurementVectorSize( 3 );
  
  MeasurementVectorType mv;
  mv[0] = 181; mv[1] = 181; mv[2] = 173;
  sample->PushBack(mv);
  mv[0] = 165.376; mv[1] = 166.376; mv[2] = 160.376;
  sample->PushBack(mv);
  mv[0] = 139.64; mv[1] = 140.64; mv[2] = 134.64;
  sample->PushBack(mv);
  mv[0] = 178.688; mv[1] = 181.416; mv[2] = 174.552;
  sample->PushBack(mv);
  mv[0] = 175.328; mv[1] = 176.328; mv[2] = 168.328;
  sample->PushBack(mv);
  mv[0] = 180.52; mv[1] = 181.52; mv[2] = 173.52;
  sample->PushBack(mv);
  mv[0] = 174.456; mv[1] = 176.456; mv[2] = 165.456;
  sample->PushBack(mv);
  mv[0] = 180; mv[1] = 177; mv[2] = 172;
  sample->PushBack(mv);
  mv[0] = 179.696; mv[1] = 176.696; mv[2] = 171.696;
  sample->PushBack(mv);
  
  mv[0] = 195.88; mv[1] = 191.88; mv[2] = 188.88;
  sample->PushBack(mv);
  mv[0] = 207.36; mv[1] = 206.36; mv[2] = 202.36;
  sample->PushBack(mv);
  mv[0] = 176.672; mv[1] = 176.008; mv[2] = 171.336;
  sample->PushBack(mv);
  mv[0] = 180.176; mv[1] = 181.176; mv[2] = 174.088;
  sample->PushBack(mv);
  mv[0] = 171.304; mv[1] = 172.304; mv[2] = 166.304;
  sample->PushBack(mv);
  mv[0] = 161; mv[1] = 162; mv[2] = 156.032;
  sample->PushBack(mv);
  mv[0] = 166.96; mv[1] = 167.96; mv[2] = 162.96;
  sample->PushBack(mv);
  mv[0] = 169; mv[1] = 170; mv[2] = 164.392;
  sample->PushBack(mv);
  mv[0] = 165.104; mv[1] = 166.104; mv[2] = 160.104;
  sample->PushBack(mv);
  mv[0] = 170.264; mv[1] = 171.264; mv[2] = 164.896;
  sample->PushBack(mv);
  mv[0] = 165.472; mv[1] = 165.944; mv[2] = 158.888;
  sample->PushBack(mv);

  typedef itk::Statistics::WeightedCentroidKdTreeGenerator< SampleType > TreeGeneratorType;
  TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();

  treeGenerator->SetSample( sample );
  treeGenerator->SetBucketSize( 16 );
  std::cout << "Tree measurement vector size: " << treeGenerator->GetMeasurementVectorSize() << std::endl;
  treeGenerator->Update();

  typedef TreeGeneratorType::KdTreeType TreeType;
  typedef itk::Statistics::KdTreeBasedKmeansEstimator<TreeType> EstimatorType;
  EstimatorType::Pointer estimator = EstimatorType::New();

  EstimatorType::ParametersType initialMeans(2*3);
  initialMeans.Fill(165.0f);
  initialMeans[3] = 200;
  initialMeans[4] = 200;
  initialMeans[5] = 200;

  estimator->SetParameters( initialMeans );
  estimator->SetKdTree( treeGenerator->GetOutput() );
  estimator->SetMaximumIteration( 200 );
  estimator->SetCentroidPositionChangesThreshold(0.0);
  estimator->StartOptimization();

  EstimatorType::ParametersType estimatedMeans = estimator->GetParameters();
  std::cout << "estimatedMeans: " << estimatedMeans << std::endl;

  return EXIT_SUCCESS;
}
