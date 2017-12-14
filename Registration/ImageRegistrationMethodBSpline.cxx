#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkTimeProbesCollectorBase.h"
#include "itkSpatialObjectToImageFilter.h"
#include "itkEllipseSpatialObject.h"

#include "itkBSplineTransform.h"
#include "itkLBFGSOptimizer.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSquaredDifferenceImageFilter.h"

#include "QuickView.h"

const    unsigned int    ImageDimension = 2;
typedef  float           PixelType;

typedef itk::Image< PixelType, ImageDimension >  ImageType;

static void CreateEllipseImage(ImageType::Pointer image);
static void CreateCircleImage(ImageType::Pointer image);

int main( int /*argc*/, char * /*argv*/[] )
{

  const unsigned int SpaceDimension = ImageDimension;
  const unsigned int SplineOrder = 3;
  typedef double CoordinateRepType;

  typedef itk::BSplineTransform<
                            CoordinateRepType,
                            SpaceDimension,
                            SplineOrder >     TransformType;

  typedef itk::LBFGSOptimizer       OptimizerType;


  typedef itk::MeanSquaresImageToImageMetric<
                                    ImageType,
                                    ImageType >    MetricType;

  typedef itk:: LinearInterpolateImageFunction<
                                    ImageType,
                                    double          >    InterpolatorType;

  typedef itk::ImageRegistrationMethod<
                                    ImageType,
                                    ImageType >    RegistrationType;

  MetricType::Pointer         metric        = MetricType::New();
  OptimizerType::Pointer      optimizer     = OptimizerType::New();
  InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
  RegistrationType::Pointer   registration  = RegistrationType::New();



  // The old registration framework has problems with multi-threading
  // For now, we set the number of threads to 1
  registration->SetNumberOfThreads(1);

  registration->SetMetric(        metric        );
  registration->SetOptimizer(     optimizer     );
  registration->SetInterpolator(  interpolator  );

  TransformType::Pointer  transform = TransformType::New();
  registration->SetTransform( transform );

  // Create the synthetic images
  ImageType::Pointer  fixedImage  = ImageType::New();
  CreateCircleImage(fixedImage);

  ImageType::Pointer movingImage = ImageType::New();
  CreateEllipseImage(movingImage);

  // Setup the registration
  registration->SetFixedImage(  fixedImage   );
  registration->SetMovingImage(   movingImage);

  ImageType::RegionType fixedRegion = fixedImage->GetBufferedRegion();
  registration->SetFixedImageRegion( fixedRegion );

  //  Here we define the parameters of the BSplineDeformableTransform grid.  We
  //  arbitrarily decide to use a grid with $5 \times 5$ nodes within the image.
  //  The reader should note that the BSpline computation requires a
  //  finite support region ( 1 grid node at the lower borders and 2
  //  grid nodes at upper borders). Therefore in this example, we set
  //  the grid size to be $8 \times 8$ and place the grid origin such that
  //  grid node (1,1) coincides with the first pixel in the fixed image.

  TransformType::PhysicalDimensionsType   fixedPhysicalDimensions;
  TransformType::MeshSizeType             meshSize;
  for( unsigned int i=0; i < ImageDimension; i++ )
    {
    fixedPhysicalDimensions[i] = fixedImage->GetSpacing()[i] *
      static_cast<double>(
        fixedImage->GetLargestPossibleRegion().GetSize()[i] - 1 );
    }
  unsigned int numberOfGridNodesInOneDimension = 5;
  meshSize.Fill( numberOfGridNodesInOneDimension - SplineOrder );
  transform->SetTransformDomainOrigin( fixedImage->GetOrigin() );
  transform->SetTransformDomainPhysicalDimensions( fixedPhysicalDimensions );
  transform->SetTransformDomainMeshSize( meshSize );
  transform->SetTransformDomainDirection( fixedImage->GetDirection() );

  typedef TransformType::ParametersType     ParametersType;

  const unsigned int numberOfParameters =
               transform->GetNumberOfParameters();

  ParametersType parameters( numberOfParameters );

  parameters.Fill( 0.0 );

  transform->SetParameters( parameters );

  //  We now pass the parameters of the current transform as the initial
  //  parameters to be used when the registration process starts.

  registration->SetInitialTransformParameters( transform->GetParameters() );

  std::cout << "Intial Parameters = " << std::endl;
  std::cout << transform->GetParameters() << std::endl;

  //  Next we set the parameters of the LBFGS Optimizer.

  optimizer->SetGradientConvergenceTolerance( 0.05 );
  optimizer->SetLineSearchAccuracy( 0.9 );
  optimizer->SetDefaultStepLength( .5 );
  optimizer->TraceOn();
  optimizer->SetMaximumNumberOfFunctionEvaluations( 1000 );

  std::cout << std::endl << "Starting Registration" << std::endl;

  try
    {
    registration->Update();
    std::cout << "Optimizer stop condition = "
              << registration->GetOptimizer()->GetStopConditionDescription()
              << std::endl;
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  OptimizerType::ParametersType finalParameters =
                    registration->GetLastTransformParameters();

  std::cout << "Last Transform Parameters" << std::endl;
  std::cout << finalParameters << std::endl;

  transform->SetParameters( finalParameters );

  typedef itk::ResampleImageFilter<
                            ImageType,
                            ImageType >    ResampleFilterType;

  ResampleFilterType::Pointer resample = ResampleFilterType::New();

  resample->SetTransform( transform );
  resample->SetInput( movingImage );

  resample->SetSize(    fixedImage->GetLargestPossibleRegion().GetSize() );
  resample->SetOutputOrigin(  fixedImage->GetOrigin() );
  resample->SetOutputSpacing( fixedImage->GetSpacing() );
  resample->SetOutputDirection( fixedImage->GetDirection() );
  resample->SetDefaultPixelValue( 100 );

  typedef  unsigned char  OutputPixelType;

  typedef itk::Image< OutputPixelType, ImageDimension > OutputImageType;

  typedef itk::CastImageFilter<
                        ImageType,
                        OutputImageType > CastFilterType;

  typedef itk::ImageFileWriter< OutputImageType >  OutputWriterType;

  OutputWriterType::Pointer      writer =  OutputWriterType::New();
  CastFilterType::Pointer  caster =  CastFilterType::New();


  writer->SetFileName("output.png");

  caster->SetInput( resample->GetOutput() );
  writer->SetInput( caster->GetOutput()   );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  QuickView viewer;
  viewer.AddImage(
    fixedImage.GetPointer(),true,
    "Fixed Image");
  viewer.AddImage(
    movingImage.GetPointer(),true,
    "Moving Image");
  viewer.AddImage(
    resample->GetOutput(),true,
    "Resampled Moving Image");

  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateEllipseImage(ImageType::Pointer image)
{
  typedef itk::EllipseSpatialObject< ImageDimension >   EllipseType;

  typedef itk::SpatialObjectToImageFilter<
    EllipseType, ImageType >   SpatialObjectToImageFilterType;

  SpatialObjectToImageFilterType::Pointer imageFilter =
    SpatialObjectToImageFilterType::New();

  ImageType::SizeType size;
  size[ 0 ] =  100;
  size[ 1 ] =  100;

  imageFilter->SetSize( size );

  ImageType::SpacingType spacing;
  spacing.Fill(1);
  imageFilter->SetSpacing(spacing);

  EllipseType::Pointer ellipse    = EllipseType::New();
  EllipseType::ArrayType radiusArray;
  radiusArray[0] = 10;
  radiusArray[1] = 20;
  ellipse->SetRadius(radiusArray);

  typedef EllipseType::TransformType                 TransformType;
  TransformType::Pointer transform = TransformType::New();
  transform->SetIdentity();

  TransformType::OutputVectorType  translation;
  TransformType::CenterType        center;

  translation[ 0 ] =  65;
  translation[ 1 ] =  45;
  transform->Translate( translation, false );

  ellipse->SetObjectToParentTransform( transform );

  imageFilter->SetInput(ellipse);

  ellipse->SetDefaultInsideValue(255);
  ellipse->SetDefaultOutsideValue(0);
  imageFilter->SetUseObjectValue( true );
  imageFilter->SetOutsideValue( 0 );

  imageFilter->Update();

  image->Graft(imageFilter->GetOutput());

}

void CreateCircleImage(ImageType::Pointer image)
{
 typedef itk::EllipseSpatialObject< ImageDimension >   EllipseType;

  typedef itk::SpatialObjectToImageFilter<
    EllipseType, ImageType >   SpatialObjectToImageFilterType;

  SpatialObjectToImageFilterType::Pointer imageFilter =
    SpatialObjectToImageFilterType::New();

  ImageType::SizeType size;
  size[ 0 ] =  100;
  size[ 1 ] =  100;

  imageFilter->SetSize( size );

  ImageType::SpacingType spacing;
  spacing.Fill(1);
  imageFilter->SetSpacing(spacing);

  EllipseType::Pointer ellipse    = EllipseType::New();
  EllipseType::ArrayType radiusArray;
  radiusArray[0] = 10;
  radiusArray[1] = 10;
  ellipse->SetRadius(radiusArray);

  typedef EllipseType::TransformType                 TransformType;
  TransformType::Pointer transform = TransformType::New();
  transform->SetIdentity();

  TransformType::OutputVectorType  translation;
  TransformType::CenterType        center;

  translation[ 0 ] =  50;
  translation[ 1 ] =  50;
  transform->Translate( translation, false );

  ellipse->SetObjectToParentTransform( transform );

  imageFilter->SetInput(ellipse);

  ellipse->SetDefaultInsideValue(255);
  ellipse->SetDefaultOutsideValue(0);
  imageFilter->SetUseObjectValue( true );
  imageFilter->SetOutsideValue( 0 );

  imageFilter->Update();

  image->Graft(imageFilter->GetOutput());
}
