#ifndef ExampleCostFunction_h
#define ExampleCostFunction_h

#include "itkSingleValuedCostFunction.h"

namespace itk
{
class ExampleCostFunction2 :
public SingleValuedCostFunction
{
public:
  /** Standard class type alias. */
  using Self = ExampleCostFunction2;
  using Superclass = SingleValuedCostFunction;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExampleCostFunction2, SingleValuedCostfunction);

  unsigned int GetNumberOfParameters(void) const override { return 2; } // itk::CostFunction

  MeasureType GetValue(const ParametersType & parameters) const override {
    return pow(parameters[0]+5, 2)+pow(parameters[1]-7, 2)+5;
  }

  void GetDerivative(const ParametersType &,
                     DerivativeType & ) const override {
    throw itk::ExceptionObject( __FILE__, __LINE__, "No derivative is available for this cost function.");
  }

protected:
  ExampleCostFunction2(){};
  ~ExampleCostFunction2() override{};

private:
  ExampleCostFunction2(const Self &); //purposely not implemented
  void operator = (const Self &); //purposely not implemented
};

} // end namespace itk

#endif
