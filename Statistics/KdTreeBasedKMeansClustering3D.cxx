#include "itkDecisionRule.h"
#include "itkVector.h"
#include "itkListSample.h"
#include "itkKdTree.h"
#include "itkWeightedCentroidKdTreeGenerator.h"
#include "itkKdTreeBasedKmeansEstimator.h"
#if ITK_VERSION_MAJOR < 4
#include "itkMinimumDecisionRule2.h"
#else
#include "itkMinimumDecisionRule.h"
#endif
#include "itkEuclideanDistanceMetric.h"
#include "itkDistanceToCentroidMembershipFunction.h"
#include "itkSampleClassifierFilter.h"
#include "itkNormalVariateGenerator.h"
 
#include "vtkVersion.h"
#include "vtkActor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkVertexGlyphFilter.h"
 
int main(int, char *[])
{
  using MeasurementVectorType = itk::Vector< double, 3 >;
  using SampleType = itk::Statistics::ListSample< MeasurementVectorType >;
  SampleType::Pointer sample = SampleType::New();
 
  using NormalGeneratorType = itk::Statistics::NormalVariateGenerator;
  NormalGeneratorType::Pointer normalGenerator = NormalGeneratorType::New();
 
  normalGenerator->Initialize( 101 );
 
  MeasurementVectorType mv;
  double mean = 100;
  double standardDeviation = 30;
  for ( unsigned int i = 0; i < 100; ++i )
    {
    mv[0] = ( normalGenerator->GetVariate() * standardDeviation ) + mean;
    mv[1] = ( normalGenerator->GetVariate() * standardDeviation ) + mean;
    mv[2] = ( normalGenerator->GetVariate() * standardDeviation ) + mean;
    sample->PushBack( mv );
    }
 
  normalGenerator->Initialize( 3024 );
  mean = 200;
  standardDeviation = 30;
  for ( unsigned int i = 0; i < 100; ++i )
    {
    mv[0] = ( normalGenerator->GetVariate() * standardDeviation ) + mean;
    mv[1] = ( normalGenerator->GetVariate() * standardDeviation ) + mean;
    mv[2] = ( normalGenerator->GetVariate() * standardDeviation ) + mean;
    sample->PushBack( mv );
    }
 
  using TreeGeneratorType = itk::Statistics::WeightedCentroidKdTreeGenerator< SampleType >;
  TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();
 
  treeGenerator->SetSample( sample );
  treeGenerator->SetBucketSize( 16 );
  treeGenerator->Update();
 
  using TreeType = TreeGeneratorType::KdTreeType;
  using EstimatorType = itk::Statistics::KdTreeBasedKmeansEstimator<TreeType>;
  EstimatorType::Pointer estimator = EstimatorType::New();
 
  EstimatorType::ParametersType initialMeans(6);
  initialMeans[0] = 0.0; // Cluster 1, mean[0]
  initialMeans[1] = 0.0; // Cluster 1, mean[1]
  initialMeans[2] = 0.0; // Cluster 1, mean[2]
  initialMeans[3] = 5.0; // Cluster 2, mean[0]
  initialMeans[4] = 5.0; // Cluster 2, mean[1]
  initialMeans[5] = 5.0; // Cluster 2, mean[2]
 
  estimator->SetParameters( initialMeans );
  estimator->SetKdTree( treeGenerator->GetOutput() );
  estimator->SetMaximumIteration( 200 );
  estimator->SetCentroidPositionChangesThreshold(0.0);
  estimator->StartOptimization();
 
  EstimatorType::ParametersType estimatedMeans = estimator->GetParameters();
 
  for ( unsigned int i = 0; i < 6; i+=2 )
    {
    std::cout << "cluster[" << i << "] " << std::endl;
    std::cout << "    estimated mean : " << estimatedMeans[i] << " , " << estimatedMeans[i+1] << std::endl;
    }
 
  using MembershipFunctionType = itk::Statistics::DistanceToCentroidMembershipFunction< MeasurementVectorType >;
  using MembershipFunctionPointer = MembershipFunctionType::Pointer;
 
#if ITK_VERSION_MAJOR < 4
  using DecisionRuleType = itk::Statistics::MinimumDecisionRule2;
#else
  using DecisionRuleType = itk::Statistics::MinimumDecisionRule;
#endif
  DecisionRuleType::Pointer decisionRule = DecisionRuleType::New();
 
  using ClassifierType = itk::Statistics::SampleClassifierFilter< SampleType >;
  ClassifierType::Pointer classifier = ClassifierType::New();
 
  classifier->SetDecisionRule(decisionRule);
  classifier->SetInput( sample );
  classifier->SetNumberOfClasses( 2 );
 
  using ClassLabelVectorObjectType = ClassifierType::ClassLabelVectorObjectType;
  using ClassLabelVectorType = ClassifierType::ClassLabelVectorType;
  using MembershipFunctionVectorObjectType = ClassifierType::MembershipFunctionVectorObjectType;
  using MembershipFunctionVectorType = ClassifierType::MembershipFunctionVectorType;
 
  ClassLabelVectorObjectType::Pointer  classLabelsObject = ClassLabelVectorObjectType::New();
  classifier->SetClassLabels( classLabelsObject );
 
  ClassLabelVectorType &  classLabelsVector = classLabelsObject->Get();
  classLabelsVector.push_back( 100 );
  classLabelsVector.push_back( 200 );
 
 
  MembershipFunctionVectorObjectType::Pointer membershipFunctionsObject =
    MembershipFunctionVectorObjectType::New();
  classifier->SetMembershipFunctions( membershipFunctionsObject );
 
  MembershipFunctionVectorType &  membershipFunctionsVector = membershipFunctionsObject->Get();
 
  MembershipFunctionType::CentroidType origin( sample->GetMeasurementVectorSize() );
  int index = 0;
  for ( unsigned int i = 0; i < 2; i++ )
    {
    MembershipFunctionPointer membershipFunction = MembershipFunctionType::New();
    for ( unsigned int j = 0; j < sample->GetMeasurementVectorSize(); j++ )
      {
      origin[j] = estimatedMeans[index++];
      }
    membershipFunction->SetCentroid( origin );
    membershipFunctionsVector.push_back( membershipFunction.GetPointer() );
    }
 
  classifier->Update();
 
  const ClassifierType::MembershipSampleType* membershipSample = classifier->GetOutput();
  ClassifierType::MembershipSampleType::ConstIterator iter = membershipSample->Begin();
 
  while ( iter != membershipSample->End() )
    {
    std::cout << "measurement vector = " << iter.GetMeasurementVector()
              << "class label = " << iter.GetClassLabel()
              << std::endl;
    ++iter;
    }
 
  // Visualize
  vtkSmartPointer<vtkPoints> points1 =
    vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPoints> points2 =
    vtkSmartPointer<vtkPoints>::New();
 
  iter = membershipSample->Begin();
  while ( iter != membershipSample->End() )
    {
    if(iter.GetClassLabel() == 100)
      {
      points1->InsertNextPoint(
        iter.GetMeasurementVector()[0],
        iter.GetMeasurementVector()[1],
        iter.GetMeasurementVector()[2]);
      }
    else
      {
      points2->InsertNextPoint(
        iter.GetMeasurementVector()[0],
        iter.GetMeasurementVector()[1],
        iter.GetMeasurementVector()[2]);
      }
    ++iter;
    }
 
  vtkSmartPointer<vtkPolyData> polyData1 =
    vtkSmartPointer<vtkPolyData>::New();
  polyData1->SetPoints(points1);
  vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter1 =
    vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  glyphFilter1->SetInputConnection(polyData1->GetProducerPort());
#else
  glyphFilter1->SetInputData(polyData1);
#endif
  glyphFilter1->Update();
  vtkSmartPointer<vtkPolyDataMapper> mapper1 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper1->SetInputConnection(glyphFilter1->GetOutputPort());
  vtkSmartPointer<vtkActor> actor1 =
    vtkSmartPointer<vtkActor>::New();
  actor1->GetProperty()->SetColor(0,1,0);
  actor1->GetProperty()->SetPointSize(3);
  actor1->SetMapper(mapper1);
 
  vtkSmartPointer<vtkPolyData> polyData2 =
    vtkSmartPointer<vtkPolyData>::New();
  polyData2->SetPoints(points2);
  vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter2 =
    vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  glyphFilter2->SetInputConnection(polyData2->GetProducerPort());
#else
  glyphFilter2->SetInputData(polyData2);
#endif
  glyphFilter2->Update();
  vtkSmartPointer<vtkPolyDataMapper> mapper2 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper2->SetInputConnection(glyphFilter2->GetOutputPort());
  vtkSmartPointer<vtkActor> actor2 =
    vtkSmartPointer<vtkActor>::New();
  actor2->GetProperty()->SetColor(1,0,0);
  actor2->GetProperty()->SetPointSize(3);
  actor2->SetMapper(mapper2);
 
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(300,300);
 
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(renderer);
 
  renderer->AddActor(actor1);
  renderer->AddActor(actor2);
  renderer->ResetCamera();
  
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
    vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
 
  renderWindowInteractor->SetInteractorStyle(style);
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}
