// Include the Levenberg-Marquardt optimizer and a custom cost function
#include "itkLevenbergMarquardtOptimizer.h"
#include "itkExampleCostFunction.h"

// Typedef the optimizer and cost function, for convenience
typedef itk::LevenbergMarquardtOptimizer OptimizerType;
typedef itk::ExampleCostFunction         CostType;

int main(int, char *[])
{

  // Instantiate the cost function and optimizer
  CostType::Pointer cost = CostType::New();
  OptimizerType::Pointer optimizer = OptimizerType::New();

  optimizer->SetNumberOfIterations( 100 );
  optimizer->UseCostFunctionGradientOff();
  optimizer->SetCostFunction( cost );

  // This is the initial guess for the parameter values, which we set to one
  CostType::ParametersType p(cost->GetNumberOfParameters());
  p.Fill( 1 );
  optimizer->SetInitialPosition(p);
  optimizer->StartOptimization();

  // Print out some information about the optimization
  // The parameters come out to be near to, but not exactly [5.5, 0.5]
  std::cout << "Position: " << optimizer->GetCurrentPosition() << std::endl; 
  std::cout << "Value: " << optimizer->GetValue() << std::endl;
  
  return EXIT_SUCCESS;

}
