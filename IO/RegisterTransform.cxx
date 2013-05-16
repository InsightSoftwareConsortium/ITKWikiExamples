#include "itkTransformFileReader.h"
#include "itkTransformFactoryBase.h"
#include "itkTransformFactory.h"
#include "itkMatrixOffsetTransformBase.h"

int main(int argc, char *argv[])
{
  std::string fileName;
  if(argc == 1) // No arguments were provided
  {
    fileName = "test.tfm";
  }
  else
  {
    fileName = argv[1];
  }

  typedef itk::MatrixOffsetTransformBase< double, 3, 3 > MatrixOffsetTransformType;
  itk::TransformFactory<MatrixOffsetTransformType>::RegisterTransform();

  itk::TransformFileReader::Pointer reader = itk::TransformFileReader::New();
  reader->SetFileName(fileName);
  reader->Update();

  std::cout << *(reader->GetTransformList()->begin()) << std::endl;

  return EXIT_SUCCESS;
}
