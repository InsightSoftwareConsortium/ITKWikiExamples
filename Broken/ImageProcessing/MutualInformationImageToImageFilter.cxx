#include "itkMutualInformationImageToImageMetric.h"
#include "itkRandomImageSource.h"
#include "itkTranslationTransform.h"
#include "itkLinearInterpolateImageFunction.h"

//typedef itk::Image< unsigned char, 2>  ImageType;
typedef itk::Image< float, 2> ImageType;

int main( int argc, char *argv[] )
{
  itk::Size<2> size;
  size.Fill(10);
 
  itk::RandomImageSource<ImageType>::Pointer randomImageSource1 = itk::RandomImageSource<ImageType>::New();
  randomImageSource1->SetNumberOfThreads(1); // to produce non-random results
  randomImageSource1->SetSize(size);
  randomImageSource1->SetMin(0);
  randomImageSource1->SetMax(255);
  randomImageSource1->Update();
  
  ImageType::Pointer fixedImage = randomImageSource1->GetOutput();
  
  itk::RandomImageSource<ImageType>::Pointer randomImageSource2 = itk::RandomImageSource<ImageType>::New();
  randomImageSource2->SetNumberOfThreads(1); // to produce non-random results
  randomImageSource2->SetSize(size);
  randomImageSource2->SetMin(0);
  randomImageSource2->SetMax(255);
  randomImageSource2->Update();

  //ImageType::Pointer movingImage = randomImageSource2->GetOutput();
  ImageType::Pointer movingImage = randomImageSource1->GetOutput();
  
  typedef itk::TranslationTransform<double, 2> TranslationTransformType; // This cannot be float for some reason?
  TranslationTransformType::Pointer transform = TranslationTransformType::New();
  
  typedef itk::MutualInformationImageToImageMetric<ImageType, ImageType >    MetricType;

  MetricType::Pointer metric = MetricType::New();
  
  metric->SetTransform(transform);

  metric->SetFixedImageStandardDeviation(  4 );
  metric->SetMovingImageStandardDeviation( 4 );

  metric->SetFixedImage(fixedImage);
  metric->SetMovingImage(movingImage);
  
  metric->SetFixedImageRegion(fixedImage->GetLargestPossibleRegion());
  
  itk::LinearInterpolateImageFunction<ImageType, double>::Pointer interpolator = itk::LinearInterpolateImageFunction<ImageType, double>::New();
  interpolator->SetInputImage(fixedImage);
  metric->SetInterpolator(interpolator);
  
  TranslationTransformType::ParametersType parameters;
  parameters.SetSize(2);
  parameters.Fill(0);
  std::cout << "parameters: " << parameters << std::endl;
  
  MetricType::MeasureType value = metric->GetValue(parameters);
  
  std::cout << "Value: " << value << std::endl;
  
  return EXIT_SUCCESS;
}
