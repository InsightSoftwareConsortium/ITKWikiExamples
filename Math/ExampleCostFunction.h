#ifndef itkExampleCostFunction_h
#define itkExampleCostFunction_h

#include "itkSingleValuedCostFunction.h"

namespace itk
{
class ExampleCostFunction :
public SingleValuedCostFunction
{
public:
  /** Standard class typedefs. */
  typedef ExampleCostFunction      Self;
  typedef SingleValuedCostFunction Superclass;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExampleCostFunction, SingleValuedCostfunction);

  unsigned int GetNumberOfParameters(void) const { return 2; } // itk::CostFunction

  MeasureType GetValue(const ParametersType & parameters) const {
    return pow(parameters[0]+5, 2)+pow(parameters[1]-7, 2)+5;
  }

  void GetDerivative(const ParametersType & parameters,
                     DerivativeType & derivative ) const {
    throw itk::ExceptionObject( __FILE__, __LINE__, "No derivative is available for this cost function.");
  }

protected:
  ExampleCostFunction(){};
  ~ExampleCostFunction(){};

private:
  ExampleCostFunction(const Self &); //purposely not implemented
  void operator = (const Self &); //purposely not implemented
};

} // end namespace itk

#endif
