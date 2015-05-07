  // Set properties pertinent to convergence
  optimizer->SetMaximumNumberOfIterations( 100 );
  optimizer->SetParametersConvergenceTolerance( 0.01 );
  optimizer->SetFunctionConvergenceTolerance( 0.01 );

  // Instantiate the cost function
  // The cost function is a 2D paraboloid in the x-y plane
  // with the equation f(x,y) = (x+5)^2+(y-7)^2 + 5 
  // and a global minimum at (x,y) = (-5, 7)
  CostType::Pointer cost = CostType::New();

  // Assign the cost function to the optimizer
  optimizer->SetCostFunction( cost.GetPointer() );
  
  // Set the initial parameters of the cost function
  OptimizerType::ParametersType initial(2);
  initial[0] = 123;
  initial[1] = -97.4;
  optimizer->SetInitialPosition( initial );

  // Begin the optimization!
  optimizer->StartOptimization();

  // Print out some information about the optimization
  std::cout << "Position: " << optimizer->GetCurrentPosition() << std::endl;
  std::cout << "Value: " << optimizer->GetValue() << std::endl;

  // As expected, the position is near to (-5, 7) and the value to 5
  // Position: [-5.003825599641884, 6.998563761340231]
  // Value: 5.00002
  return EXIT_SUCCESS;

}
