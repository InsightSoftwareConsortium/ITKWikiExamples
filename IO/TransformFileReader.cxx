#include "itkTransformFileReader.h"
#include "itkTransformFactoryBase.h"

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

  // Register default transforms
  itk::TransformFactoryBase::RegisterDefaultTransforms();

  itk::TransformFileReader::Pointer reader = itk::TransformFileReader::New();
  reader->SetFileName(fileName);
  reader->Update();

  return EXIT_SUCCESS;
}
