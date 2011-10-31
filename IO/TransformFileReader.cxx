#include "itkTransformFileReader.h"
#include "itkTransformFactoryBase.h"

int main(int, char *[])
{
  // Register default transforms
  itk::TransformFactoryBase::RegisterDefaultTransforms();

  itk::TransformFileReader::Pointer reader = itk::TransformFileReader::New();
  reader->SetFileName("test.txt");
  reader->Update();

  return EXIT_SUCCESS;
}
