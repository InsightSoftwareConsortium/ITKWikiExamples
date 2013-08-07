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

#if (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR >= 5) || ITK_VERSION_MAJOR > 4
  itk::TransformFileReaderTemplate<float>::Pointer reader =
    itk::TransformFileReaderTemplate<float>::New();
#else
  itk::TransformFileReader::Pointer writer = itk::TransformFileReader::New();
#endif
  reader->SetFileName(fileName);
  reader->Update();

  // Display the transform
  std::cout << *(reader->GetTransformList()->begin()) << std::endl;

  return EXIT_SUCCESS;
}
