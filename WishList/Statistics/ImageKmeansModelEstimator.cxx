#include "itkImage.h"
#include "itkListSample.h"
#include "itkVector.h"
#include "itkImageKmeansModelEstimator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageToListSampleAdaptor.h"
#include "itkDistanceToCentroidMembershipFunction.h"
#include "itkSampleClassifierFilter.h"
#include "itkMinimumDecisionRule.h"
#include "itkImageFileWriter.h"

typedef itk::Vector<unsigned char,3> MeasurementVectorType;
typedef itk::Image<MeasurementVectorType,2> ColorImageType;
typedef itk::Image<unsigned char,2> ScalarImageType;

static void CreateImage(ColorImageType::Pointer image);

int main(int, char* [] )
{
  // Create a demo image
  ColorImageType::Pointer image = ColorImageType::New();
  CreateImage(image);

  // Compute pixel clusters using KMeans
  typedef itk::Statistics::DistanceToCentroidMembershipFunction< MeasurementVectorType >  MembershipFunctionType ;
  typedef MembershipFunctionType::Pointer MembershipFunctionPointer ;
  typedef std::vector< MembershipFunctionPointer >  MembershipFunctionPointerVector;

  typedef itk::ImageKmeansModelEstimator<ColorImageType, MembershipFunctionType>  ImageKmeansModelEstimatorType;

  ImageKmeansModelEstimatorType::Pointer kmeansEstimator = ImageKmeansModelEstimatorType::New();
  kmeansEstimator->SetInputImage(image);
  kmeansEstimator->SetNumberOfModels(3);
  kmeansEstimator->SetThreshold(0.01 );
  kmeansEstimator->SetOffsetAdd( 0.01 );
  kmeansEstimator->SetOffsetMultiply( 0.01 );
  kmeansEstimator->SetMaxSplitAttempts( 10 );
  kmeansEstimator->Update();

  // Classify each pixel
  typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType ;
  typedef itk::Statistics::SampleClassifierFilter< SampleType > ClassifierType;
  ClassifierType::Pointer classifier = ClassifierType::New();

  typedef itk::Statistics::MinimumDecisionRule DecisionRuleType;
  DecisionRuleType::Pointer decisionRule = DecisionRuleType::New();
  
  classifier->SetDecisionRule(decisionRule);
  classifier->SetNumberOfClasses(3);

  typedef ClassifierType::ClassLabelVectorObjectType               ClassLabelVectorObjectType;
  typedef ClassifierType::ClassLabelVectorType                     ClassLabelVectorType;
  typedef ClassifierType::MembershipFunctionVectorObjectType       MembershipFunctionVectorObjectType;
  typedef ClassifierType::MembershipFunctionVectorType             MembershipFunctionVectorType;

  // Setup membership functions
  MembershipFunctionPointerVector kmeansMembershipFunctions = kmeansEstimator->GetMembershipFunctions();

  MembershipFunctionVectorObjectType::Pointer  membershipFunctionsVectorObject = MembershipFunctionVectorObjectType::New();
  classifier->SetMembershipFunctions(membershipFunctionsVectorObject);

  MembershipFunctionVectorType &  membershipFunctionsVector = membershipFunctionsVectorObject->Get();

  for(unsigned int i = 0; i < kmeansMembershipFunctions.size(); i++)
    {
    membershipFunctionsVector.push_back(kmeansMembershipFunctions[i].GetPointer());
    }

  // Setup class labels
  ClassLabelVectorObjectType::Pointer  classLabelsObject = ClassLabelVectorObjectType::New();
  classifier->SetClassLabels( classLabelsObject );

  ClassLabelVectorType &  classLabelsVector = classLabelsObject->Get();
  classLabelsVector.push_back( 50 );
  classLabelsVector.push_back( 150 );
  classLabelsVector.push_back( 250 );

  // Perform the classification
  typedef itk::Statistics::ImageToListSampleAdaptor< ColorImageType > SampleAdaptorType;
  SampleAdaptorType::Pointer sample = SampleAdaptorType::New();
  sample->SetImage(image);

  classifier->SetInput(sample);
  classifier->Update();
  
  // Prepare the output image
  ScalarImageType::Pointer outputImage = ScalarImageType::New();
  outputImage->SetRegions(image->GetLargestPossibleRegion());
  outputImage->Allocate();
  outputImage->FillBuffer(0);

  // Setup the membership iterator
  const ClassifierType::MembershipSampleType* membershipSample = classifier->GetOutput();
  ClassifierType::MembershipSampleType::ConstIterator membershipIterator = membershipSample->Begin();

  // Setup the output image iterator - this is automatically synchronized with the membership iterator since the sample is an adaptor
  itk::ImageRegionIteratorWithIndex<ScalarImageType> outputIterator(outputImage,outputImage->GetLargestPossibleRegion());
  outputIterator.GoToBegin();
  
  while(membershipIterator != membershipSample->End())
    {
    int classLabel = membershipIterator.GetClassLabel();
    //std::cout << "Class label: " << classLabel << std::endl;
    outputIterator.Set(classLabel);
    ++membershipIterator;
    ++outputIterator;
    }
    
  typedef  itk::ImageFileWriter< ColorImageType  > WriterType;
  WriterType::Pointer inputWriter = WriterType::New();
  inputWriter->SetFileName("input.mha");
  inputWriter->SetInput(image);
  inputWriter->Update();

  typedef  itk::ImageFileWriter< ScalarImageType  > ScalarWriterType;
  ScalarWriterType::Pointer outputWriter = ScalarWriterType::New();
  outputWriter->SetFileName("output.mha");
  outputWriter->SetInput(outputImage);
  outputWriter->Update();
  
  
  return EXIT_SUCCESS;
}

void CreateImage(ColorImageType::Pointer image)
{
  // Create a black image with a red square and a green square
  ColorImageType::RegionType region;
  ColorImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ColorImageType::SizeType size;
  size[0] = 200;
  size[1] = 300;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ColorImageType> imageIterator(image,region);

  itk::Vector<unsigned char, 3> redPixel;
  redPixel[0] = 255;
  redPixel[1] = 0;
  redPixel[2] = 0;

  itk::Vector<unsigned char, 3> greenPixel;
  greenPixel[0] = 0;
  greenPixel[1] = 255;
  greenPixel[2] = 0;
  
  itk::Vector<unsigned char, 3> blackPixel;
  blackPixel[0] = 0;
  blackPixel[1] = 0;
  blackPixel[2] = 0;
  
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > 100 &&
      imageIterator.GetIndex()[0] < 150 &&
      imageIterator.GetIndex()[1] > 100 &&
      imageIterator.GetIndex()[1] < 150)
      {
      imageIterator.Set(redPixel);
      }
    else if(imageIterator.GetIndex()[0] > 50 &&
      imageIterator.GetIndex()[0] < 70 &&
      imageIterator.GetIndex()[1] > 50 &&
      imageIterator.GetIndex()[1] < 70)
      {
      imageIterator.Set(greenPixel);
      }
    else
      {
      imageIterator.Set(blackPixel);
      }

    ++imageIterator;
  }
}
