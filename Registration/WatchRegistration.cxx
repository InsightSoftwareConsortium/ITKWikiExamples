#include "itkImageRegistrationMethod.h"
#include "itkAffineTransform.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkNormalizeImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkCheckerBoardImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
#include "itkCenteredTransformInitializer.h"

#include "vtkVersion.h"

#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageActor.h"
#include "vtkImageMapper3D.h"

const    unsigned int    Dimension = 2;
typedef  unsigned char   PixelType;

typedef itk::Image< PixelType, Dimension >      InputImageType;
typedef itk::Image< unsigned char, Dimension >  OutputImageType;

//  Command observer to visualize the evolution of the registration process.
//
#include "itkCommand.h"
template< typename TImage >
class IterationUpdate : public itk::Command
{
public:
  typedef IterationUpdate          Self;
  typedef itk::Command             Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );

protected:
  IterationUpdate() {};

public:
  typedef itk::Image< float, 2>                    InternalImageType;
  typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
  typedef const OptimizerType *                    OptimizerPointer;
  typedef itk::ResampleImageFilter<
                            TImage,
                            TImage >               ResampleFilterType;
  typedef itk::AffineTransform< double, 2 >        TransformType;
  typedef itk::ImageToVTKImageFilter<TImage >      ConnectorType;
  typedef itk::FlipImageFilter<TImage>             FilterType;
  void Execute(itk::Object *caller, const itk::EventObject & event) ITK_OVERRIDE
    {
    Execute( (const itk::Object *) caller, event);
    }

  void Execute(const itk::Object * object, const itk::EventObject & event) ITK_OVERRIDE
    {
    OptimizerPointer optimizer = static_cast< OptimizerPointer >( object );
    if( !(itk::IterationEvent().CheckEvent( &event )) )
      {
      return;
      }

    m_Transform->SetParameters(optimizer->GetCurrentPosition());
    m_Filter->Update();
    m_Connector->SetInput(m_Filter->GetOutput());
    m_Connector->Update();

#if VTK_MAJOR_VERSION <= 5
    m_ImageActor->SetInput(m_Connector->GetOutput());
#else
    m_Connector->Update();
    m_ImageActor->GetMapper()->SetInputData(m_Connector->GetOutput());
#endif
    m_RenderWindow->Render();
    }
  void SetTransform(TransformType::Pointer &transform)
    {
    m_Transform = transform;
    }
  void SetFilter(typename FilterType::Pointer &filter)
    {
    m_Filter = filter;
    }
  void SetConnector(typename ConnectorType::Pointer &connector)
    {
    m_Connector = connector;
    }
  void SetImageActor(vtkImageActor *actor)
    {
    m_ImageActor = actor;
    }
  void SetRenderWindow(vtkRenderWindow *renderWindow)
    {
    m_RenderWindow = renderWindow;
    }
  TransformType::Pointer               m_Transform;
  typename FilterType::Pointer         m_Filter;
  typename ConnectorType::Pointer      m_Connector;
  vtkImageActor *                      m_ImageActor;
  vtkRenderWindow *                    m_RenderWindow;
};

int main( int argc, char *argv[] )
{
  InputImageType::Pointer  fixedImage = InputImageType::New();
  InputImageType::Pointer movingImage = InputImageType::New();

  if (argc > 2)
    {
    typedef itk::ImageFileReader<InputImageType> ReaderType;
    ReaderType::Pointer fixedReader = ReaderType::New();
    fixedReader->SetFileName( argv[1] );
    fixedReader->Update();
    fixedImage = fixedReader->GetOutput();

    ReaderType::Pointer movingReader = ReaderType::New();
    movingReader->SetFileName( argv[2] );
    movingReader->Update();
    movingImage = movingReader->GetOutput();
    }
  else
    {
    std::cout << "Usage: " << argv[0] << " fixedImage movingImage" << std::endl;
    return EXIT_FAILURE;
    }

  // Use floats internally
  typedef itk::Image< float, Dimension> InternalImageType;

  // Normalize the images
  typedef itk::NormalizeImageFilter<InputImageType, InternalImageType> NormalizeFilterType;
  NormalizeFilterType::Pointer fixedNormalizer =
    NormalizeFilterType::New();
  NormalizeFilterType::Pointer movingNormalizer =
    NormalizeFilterType::New();

  fixedNormalizer->SetInput(  fixedImage);
  movingNormalizer->SetInput( movingImage);

  // Smooth the normalized images
  typedef itk::DiscreteGaussianImageFilter<InternalImageType,InternalImageType> GaussianFilterType;
  GaussianFilterType::Pointer fixedSmoother  =
    GaussianFilterType::New();
  GaussianFilterType::Pointer movingSmoother =
    GaussianFilterType::New();

  fixedSmoother->SetVariance( 3.0 );
  fixedSmoother->SetInput( fixedNormalizer->GetOutput() );
  movingSmoother->SetVariance( 3.0 );
  movingSmoother->SetInput( movingNormalizer->GetOutput() );

  // Set up registration
  typedef itk::AffineTransform< double, Dimension >           TransformType;
  typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
  typedef itk::LinearInterpolateImageFunction<
                                    InternalImageType,
                                    double             >      InterpolatorType;
  typedef itk::MattesMutualInformationImageToImageMetric<
                                          InternalImageType,
                                          InternalImageType >    MetricType;
  typedef itk::ImageRegistrationMethod<
                                    InternalImageType,
                                    InternalImageType >       RegistrationType;
  typedef itk::CenteredTransformInitializer<
                                  TransformType,
                                  InputImageType,
                                  InputImageType >
                                            InitializerType;

  InitializerType::Pointer    initializer = InitializerType::New();
  TransformType::Pointer      transform     = TransformType::New();
  OptimizerType::Pointer      optimizer     = OptimizerType::New();
  InterpolatorType::Pointer   interpolator  = InterpolatorType::New();
  RegistrationType::Pointer   registration  = RegistrationType::New();

  // Set up the registration framework
  initializer->SetFixedImage( fixedImage );
  initializer->SetMovingImage( movingImage );
  initializer->SetTransform( transform );

  transform->SetIdentity();
  initializer->GeometryOn();
  initializer->InitializeTransform();

  registration->SetOptimizer(     optimizer     );
  registration->SetTransform(     transform     );
  registration->SetInterpolator(  interpolator  );

  MetricType::Pointer         metric        = MetricType::New();

  registration->SetMetric( metric );
  registration->SetFixedImage( fixedSmoother->GetOutput() );
  registration->SetMovingImage(movingSmoother->GetOutput() );

  // Update to get the size of the region
  fixedNormalizer->Update();
  InputImageType::RegionType fixedImageRegion =
       fixedNormalizer->GetOutput()->GetBufferedRegion();
  registration->SetFixedImageRegion( fixedImageRegion );

  typedef RegistrationType::ParametersType ParametersType;
  ParametersType initialParameters( transform->GetNumberOfParameters() );

  // rotation matrix (identity)
  initialParameters[0] = 1.0;  // R(0,0)
  initialParameters[1] = 0.0;  // R(0,1)
  initialParameters[2] = 0.0;  // R(1,0)
  initialParameters[3] = 1.0;  // R(1,1)

  // translation vector
  initialParameters[4] = 0.0;
  initialParameters[5] = 0.0;

  registration->SetInitialTransformParameters( initialParameters );

  const unsigned int numberOfPixels = fixedImageRegion.GetNumberOfPixels();
  const unsigned int numberOfSamples =
                        static_cast< unsigned int >( numberOfPixels * 0.05 );

  metric->SetNumberOfHistogramBins( 26 );
  metric->SetNumberOfSpatialSamples( numberOfSamples );

  optimizer->MinimizeOn();
  optimizer->SetMaximumStepLength( 0.500 );
  optimizer->SetMinimumStepLength( 0.001 );
  optimizer->SetNumberOfIterations( 1000 );

  const unsigned int numberOfParameters =  transform->GetNumberOfParameters();
  typedef OptimizerType::ScalesType       OptimizerScalesType;
  OptimizerScalesType optimizerScales( numberOfParameters );
  double translationScale = 1.0 / 1000.0;
  optimizerScales[0] =  1.0;
  optimizerScales[1] =  1.0;
  optimizerScales[2] =  1.0;
  optimizerScales[3] =  1.0;
  optimizerScales[4] =  translationScale;
  optimizerScales[5] =  translationScale;

  optimizer->SetScales( optimizerScales );

  TransformType::Pointer finalTransform =
    TransformType::New();
  finalTransform->SetParameters( initialParameters );
  finalTransform->SetFixedParameters( transform->GetFixedParameters() );

  typedef itk::ResampleImageFilter< InputImageType, InputImageType >
    ResampleFilterType;
  ResampleFilterType::Pointer resample =
    ResampleFilterType::New();
  resample->SetTransform( finalTransform );
  resample->SetInput( movingImage);
  resample->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
  resample->SetOutputOrigin( fixedImage->GetOrigin() );
  resample->SetOutputSpacing( fixedImage->GetSpacing() );
  resample->SetOutputDirection( fixedImage->GetDirection() );
  resample->SetDefaultPixelValue( 100 );
  resample->Update();

  // Set up the visualization pipeline
  typedef itk::CheckerBoardImageFilter< InputImageType > CheckerBoardFilterType;
  CheckerBoardFilterType::Pointer checkerboard =
    CheckerBoardFilterType::New();
  CheckerBoardFilterType::PatternArrayType pattern;
  pattern[0] = 4;
  pattern[1] = 4;

  checkerboard->SetCheckerPattern( pattern );
  checkerboard->SetInput1( fixedImage);
  checkerboard->SetInput2( resample->GetOutput());

  typedef itk::FlipImageFilter< InputImageType> FlipFilterType;
  FlipFilterType::Pointer flip =
    FlipFilterType::New();
  bool flipAxes[3] = { false, true, false };
  flip->SetFlipAxes(flipAxes);
  flip->SetInput(checkerboard->GetOutput());
  flip->Update();

  // VTK visualization pipeline
  typedef itk::ImageToVTKImageFilter<InputImageType > ConnectorType;
  ConnectorType::Pointer connector =
    ConnectorType::New();
  connector->SetInput(flip->GetOutput());

  vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
#if VTK_MAJOR_VERSION <= 5
  actor->SetInput(connector->GetOutput());
#else
  connector->Update();
  actor->GetMapper()->SetInputData(connector->GetOutput());
#endif
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground(.4, .5, .6);
  renderer->AddActor(actor);
  renderWindow->SetSize(640, 480);;
  renderWindow->AddRenderer(renderer);
  renderWindow->Render();

  // Set up the iteration event observer
  IterationUpdate<InputImageType>::Pointer observer =
    IterationUpdate<InputImageType>::New();
  optimizer->AddObserver( itk::IterationEvent(), observer );

  observer->SetTransform(finalTransform);
  observer->SetFilter(flip);
  observer->SetConnector(connector);
  observer->SetImageActor(actor);
  observer->SetRenderWindow(renderWindow);
  try
    {
    registration->Update();
    std::cout << "Optimizer stop condition: "
              << registration->GetOptimizer()->GetStopConditionDescription()
              << std::endl;
    }
  catch( itk::ExceptionObject & err )
    {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "Final Transform: " << finalTransform << std::endl;

  ParametersType finalParameters = registration->GetLastTransformParameters();
  std::cout << "Final Parameters: " << finalParameters << std::endl;

  unsigned int numberOfIterations = optimizer->GetCurrentIteration();
  double bestValue = optimizer->GetValue();

  // Print out results
  std::cout << std::endl;
  std::cout << "Result = " << std::endl;
  std::cout << " Iterations    = " << numberOfIterations << std::endl;
  std::cout << " Metric value  = " << bestValue          << std::endl;
  std::cout << " Numb. Samples = " << numberOfSamples    << std::endl;

  // Interact with the image
  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  interactor->SetInteractorStyle(style);
  interactor->SetRenderWindow(renderWindow);
  interactor->Start();

  return EXIT_SUCCESS;
}
