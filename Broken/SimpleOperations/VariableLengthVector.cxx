#include <itkVariableLengthVector.h>
#include <itkVector.h>
#include <QtCore/qtextstream.h>

void VectorToVariableLengthVector();
void VariableLengthVectorToVector();

int main(int, char*[])
{
  typedef itk::VariableLengthVector<double> VectorType;
  VectorType v;
  v.SetSize(2);
  v[0] = 1;
  v[1] = 2;
  std::cout << v << std::endl;

  for(unsigned int i = 0; i < v.Size(); i++)
    {
    std::cout << v[i] << " ";
    }
  std::cout << std::endl;

  VectorToVariableLengthVector();
  VariableLengthVectorToVector();
  return EXIT_SUCCESS;
}

void VectorToVariableLengthVector()
{
  // Seems to work, but then crashes!
  
  typedef itk::Vector<double, 2> FixedVectorType;
  FixedVectorType fixedLengthVector;
  fixedLengthVector[0] = 1;
  fixedLengthVector[1] = 2;
  
  typedef itk::VariableLengthVector<double> VariableVectorType;
  VariableVectorType variableLengthVector;
  variableLengthVector.SetSize(fixedLengthVector.Size());
  variableLengthVector.SetData(fixedLengthVector.GetDataPointer(), 2i);
  std::cout << "variableLengthVector: " << variableLengthVector << std::endl;
}

void VariableLengthVectorToVector()
{
  typedef itk::VariableLengthVector<double> VariableVectorType;
  VariableVectorType variableLengthVector;
  variableLengthVector.SetSize(2);
  
  variableLengthVector[0] = 1;
  variableLengthVector[1] = 2;

  typedef itk::Vector<double, 2> FixedVectorType;
  FixedVectorType fixedLengthVector;

  // This function doesn't exist!
  //fixedLengthVector.SetData(variableLengthVector.GetDataPointer());
  //std::cout << "fixedLengthVector: " << fixedLengthVector << std::endl;
}
