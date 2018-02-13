#include "itkImageToListSampleAdaptor.h"
#include "itkImage.h"
#include "itkRandomImageSource.h"
#include "itkComposeImageFilter.h"

int main(int, char *[])
{
  using FloatImage2DType = itk::Image<float,2>;

  itk::RandomImageSource<FloatImage2DType>::Pointer random;
  random = itk::RandomImageSource<FloatImage2DType>::New();

  random->SetMin(    0.0 );
  random->SetMax( 1000.0 );

  using SpacingValueType = FloatImage2DType::SpacingValueType;
  using SizeValueType = FloatImage2DType::SizeValueType;
  using PointValueType = FloatImage2DType::PointValueType;

  SizeValueType size[2] = {20, 20};
  random->SetSize( size );

  SpacingValueType spacing[2] = {0.7, 2.1};
  random->SetSpacing( spacing );

  PointValueType origin[2] = {15, 400};
  random->SetOrigin( origin );

  using MeasurementVectorType = itk::FixedArray< float, 1 >;
  using ArrayImageType = itk::Image< MeasurementVectorType, 2 >;
  using CasterType = itk::ComposeImageFilter< FloatImage2DType, ArrayImageType >;

  CasterType::Pointer caster = CasterType::New();
  caster->SetInput( random->GetOutput() );
  caster->Update();

  using SampleType = itk::Statistics::ImageToListSampleAdaptor< ArrayImageType >;
  SampleType::Pointer sample = SampleType::New();

  sample->SetImage( caster->GetOutput() );

  SampleType::Iterator iter = sample->Begin();

  while( iter != sample->End() )
    {
    std::cout << iter.GetMeasurementVector() << std::endl;
    ++iter;
    }
    
  return EXIT_SUCCESS;
}
