#include "itkImageFileReader.h"
#include "itkEuler2DTransform.h"
#include "itkExhaustiveOptimizerv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkCenteredTransformInitializer.h"
#include "itkImageRegistrationMethodv4.h"
#include "itkImage.h"

//  Command observer to monitor the evolution of the registration process.
//
#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command
{
public:
  typedef CommandIterationUpdate   Self;
  typedef itk::Command             Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );

protected:
  CommandIterationUpdate() {};

public:
  typedef itk::ExhaustiveOptimizerv4<double> OptimizerType;
  typedef   const OptimizerType *            OptimizerPointer;

  void Execute(itk::Object *caller, const itk::EventObject & event) override
    {
    Execute( (const itk::Object *) caller, event);
    }

  void Execute(const itk::Object * object, const itk::EventObject & event) override
    {
    OptimizerPointer optimizer = static_cast< OptimizerPointer >( object );
    if( !(itk::IterationEvent().CheckEvent( &event )) )
      {
      return;
      }
    std::cout << "Iteration: ";
    std::cout << optimizer->GetCurrentIteration() << "   ";
    std::cout << optimizer->GetCurrentIndex() << "   ";
    std::cout << optimizer->GetCurrentValue() << "   ";
    std::cout << optimizer->GetCurrentPosition() << "   ";
    std::cout << std::endl;
    }
};

int main (int argc, char *argv[])
{
  if (argc < 3)
    {
    std::cout << "Usage: " << argv[0] << " fixedImage movingImage" << std::endl;
    return EXIT_FAILURE;
    }
  typedef itk::Image<double, 2>                 FixedImageType;
  typedef itk::Image<double, 2>                 MovingImageType;
  typedef itk::ImageFileReader<FixedImageType>  FixedImageReaderType;
  typedef itk::ImageFileReader<MovingImageType> MovingImageReaderType;
  typedef itk::Euler2DTransform< double >       TransformType;
  typedef itk::ExhaustiveOptimizerv4< double >  OptimizerType; 
  typedef itk::MeanSquaresImageToImageMetricv4< FixedImageType, MovingImageType >
                                                MetricType;
  typedef itk::CenteredTransformInitializer< TransformType, FixedImageType,  MovingImageType >
                                                TransformInitializerType;
  typedef itk::ImageRegistrationMethodv4< FixedImageType, MovingImageType, TransformType >
                                                RegistrationType;

  FixedImageReaderType::Pointer     fixedImageReader    = FixedImageReaderType::New();
  MovingImageReaderType::Pointer    movingImageReader    = MovingImageReaderType::New();
  FixedImageType::Pointer           fixedImage    = FixedImageType::New();
  MovingImageType::Pointer          movingImage    = MovingImageType::New();
  TransformType::Pointer            transform    = TransformType::New();
  MetricType::Pointer               metric       = MetricType::New();
  OptimizerType::Pointer            optimizer    = OptimizerType::New();
  RegistrationType::Pointer         registration = RegistrationType::New();
  TransformInitializerType::Pointer initializer  = TransformInitializerType::New();

  fixedImageReader->SetFileName (argv[1]);
  fixedImageReader->Update();
  fixedImage = fixedImageReader->GetOutput();

  movingImageReader->SetFileName (argv[2]);
  movingImageReader->Update();
  movingImage = movingImageReader->GetOutput();

  // Create the Command observer and register it with the optimizer.
  //
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  optimizer->AddObserver( itk::IterationEvent(), observer );

  unsigned int angles = 12;
  OptimizerType::StepsType steps( transform->GetNumberOfParameters() );
  steps[0] = int(angles/2);
  steps[1] = 0;
  steps[2] = 0;
  optimizer->SetNumberOfSteps( steps );

  OptimizerType::ScalesType scales( transform->GetNumberOfParameters() );
  scales[0] = 2.0 * vnl_math::pi / angles;
  scales[1] = 1.0;
  scales[2] = 1.0; 

  optimizer->SetScales( scales );

  initializer->SetTransform(   transform );
  initializer->SetFixedImage(  fixedImage );
  initializer->SetMovingImage( movingImage );
  initializer->InitializeTransform();

// Initialize registration
  registration->SetMetric(           metric );
  registration->SetOptimizer(        optimizer );
  registration->SetFixedImage(       fixedImage );
  registration->SetMovingImage(      movingImage );  
  registration->SetInitialTransform( transform );
  registration->SetNumberOfLevels(   1);
  try
    {
    registration->Update();
    std::cout << "  MinimumMetricValue: " << optimizer->GetMinimumMetricValue() << std::endl;
    std::cout << "  MaximumMetricValue: " << optimizer->GetMaximumMetricValue() << std::endl;
    std::cout << "  MinimumMetricValuePosition: " << optimizer->GetMinimumMetricValuePosition() << std::endl;
    std::cout << "  MaximumMetricValuePosition: " << optimizer->GetMaximumMetricValuePosition() << std::endl;
    std::cout << "  StopConditionDescription: " << optimizer->GetStopConditionDescription() << std::endl;
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
