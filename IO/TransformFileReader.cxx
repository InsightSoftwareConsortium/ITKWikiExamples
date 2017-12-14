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

  itk::TransformFileReaderTemplate<float>::Pointer reader =
    itk::TransformFileReaderTemplate<float>::New();
  reader->SetFileName(fileName);
  reader->Update();

  // Display the transform
  std::cout << *(reader->GetTransformList()->begin()) << std::endl;

  return EXIT_SUCCESS;
}
