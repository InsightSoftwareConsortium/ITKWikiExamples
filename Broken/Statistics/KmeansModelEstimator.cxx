#include "itkImage.h"
#include "itkListSample.h"
#include "itkVector.h"
#include "itkImageKmeansModelEstimator.h"
#include "itkImageToListSampleAdaptor.h"
#include "itkDistanceToCentroidMembershipFunction.h"
#include "itkSampleClassifierFilter.h"
#include "itkMinimumDecisionRule2.h"

#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

typedef itk::Vector<unsigned char,3> MeasurementVectorType;
typedef itk::Image<MeasurementVectorType,2> ColorImageType;
typedef itk::Image<unsigned char,2> GrayscaleImageType;

void CreateImage(ColorImageType::Pointer image);
void ITKImagetoVTKImageColor(ColorImageType::Pointer image, vtkImageData* outputImage);
void ITKImagetoVTKImageGrayscale(GrayscaleImageType::Pointer image, vtkImageData* outputImage);

int main(int, char* [] )
{
  ColorImageType::Pointer image = ColorImageType::New();
  CreateImage(image);
  
  typedef itk::Statistics::DistanceToCentroidMembershipFunction< itk::Vector<unsigned char,3> >
    MembershipFunctionType ;
  typedef MembershipFunctionType::Pointer MembershipFunctionPointer ;

  typedef std::vector< MembershipFunctionPointer >
    MembershipFunctionPointerVector;

  typedef itk::ImageKmeansModelEstimator<ColorImageType, MembershipFunctionType>
    ImageKmeansModelEstimatorType;

  ImageKmeansModelEstimatorType::Pointer
    kmeansEstimator = ImageKmeansModelEstimatorType::New();
  kmeansEstimator->SetInputImage(image);
  kmeansEstimator->SetNumberOfModels(3);
  kmeansEstimator->SetThreshold(0.01 );
  kmeansEstimator->SetOffsetAdd( 0.01 );
  kmeansEstimator->SetOffsetMultiply( 0.01 );
  kmeansEstimator->SetMaxSplitAttempts( 10 );
  kmeansEstimator->Update();
  typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType ;
  typedef itk::Statistics::SampleClassifierFilter< SampleType > ClassifierType;
  ClassifierType::Pointer classifier = ClassifierType::New();

  typedef itk::Statistics::MinimumDecisionRule2 DecisionRuleType;
  DecisionRuleType::Pointer decisionRule = DecisionRuleType::New();
  
  classifier->SetDecisionRule(decisionRule);
  
  classifier->SetNumberOfClasses(3);

  typedef ClassifierType::ClassLabelVectorObjectType               ClassLabelVectorObjectType;
  typedef ClassifierType::ClassLabelVectorType                     ClassLabelVectorType;
  typedef ClassifierType::MembershipFunctionVectorObjectType       MembershipFunctionVectorObjectType;
  typedef ClassifierType::MembershipFunctionVectorType             MembershipFunctionVectorType;

  MembershipFunctionPointerVector membershipFunctions =
  kmeansEstimator->GetMembershipFunctions();
    
  ClassLabelVectorObjectType::Pointer  classLabelsObject = ClassLabelVectorObjectType::New();
  classifier->SetClassLabels( classLabelsObject );

  ClassLabelVectorType &  classLabelsVector = classLabelsObject->Get();
  classLabelsVector.push_back( 50 );
  classLabelsVector.push_back( 150 );
  classLabelsVector.push_back( 250 );

  GrayscaleImageType::Pointer outputImage = GrayscaleImageType::New();
  outputImage->SetRegions(image->GetLargestPossibleRegion());

  itk::ImageRegionIterator<ColorImageType> imageIterator(image,image->GetLargestPossibleRegion());
  itk::ImageRegionIterator<GrayscaleImageType> outputIterator(outputImage,outputImage->GetLargestPossibleRegion());
  
  while(!imageIterator.IsAtEnd())
    {
    SampleType::Pointer sample = SampleType::New();
    sample->PushBack(imageIterator.Get());
    //sample->Get
    classifier->SetInput(sample);
    unsigned char label = static_cast<unsigned char>(classifier->GetOutput()->GetClassLabel(0));
    outputIterator.Set(label);

    ++imageIterator;

    }

  // Visualize
  vtkSmartPointer<vtkImageData> VTKImage =
    vtkSmartPointer<vtkImageData>::New();
    
  ITKImagetoVTKImageColor(image, VTKImage);

  vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
  actor->SetInput(VTKImage);

  // There will be one render window
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(300, 300);

  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(renderer);
  renderer->SetBackground(.6, .5, .4);

  renderer->AddActor(actor);

  renderer->ResetCamera();

  renderWindow->Render();

  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  interactor->SetInteractorStyle(style);

  interactor->Start();
  
  //classdist = membershipFunctions[idx]->Evaluate( outIt.Get() );
  //std::cout << "Distance of first pixel to class " << idx << " is: " << classdist << std::endl;

  return EXIT_SUCCESS;
}

void CreateImage(ColorImageType::Pointer image)
{
  // Create an image with 2 connected components
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


void ITKImagetoVTKImageColor(ColorImageType::Pointer image, vtkImageData* outputImage)
{
  outputImage->SetNumberOfScalarComponents(3);
  outputImage->SetScalarTypeToUnsignedChar();

  outputImage->SetDimensions(image->GetLargestPossibleRegion().GetSize()[0],
                             image->GetLargestPossibleRegion().GetSize()[1],
                             1);

  outputImage->AllocateScalars();

  int* dims = outputImage->GetDimensions();

  for (int y = 0; y < dims[1]; y++)
    {
    for (int x = 0; x < dims[0]; x++)
      {
      unsigned char* pixel = static_cast<unsigned char*>(outputImage->GetScalarPointer(x,y,0));
      ColorImageType::IndexType index;

      index[0] = x;
      index[1] = y;
      pixel[0] = image->GetPixel(index)[0];
      pixel[1] = image->GetPixel(index)[1];
      pixel[2] = image->GetPixel(index)[2];
      }
    }
}


void ITKImagetoVTKImageGrayscale(GrayscaleImageType::Pointer image, vtkImageData* outputImage)
{
  outputImage->SetNumberOfScalarComponents(1);
  outputImage->SetScalarTypeToUnsignedChar();

  outputImage->SetDimensions(image->GetLargestPossibleRegion().GetSize()[0],
                             image->GetLargestPossibleRegion().GetSize()[1],
                             1);

  outputImage->AllocateScalars();

  int* dims = outputImage->GetDimensions();

  for (int y = 0; y < dims[1]; y++)
    {
    for (int x = 0; x < dims[0]; x++)
      {
      unsigned char* pixel = static_cast<unsigned char*>(outputImage->GetScalarPointer(x,y,0));
      GrayscaleImageType::IndexType index;

      index[0] = x;
      index[1] = y;
      pixel[0] = image->GetPixel(index);
      }
    }
}
