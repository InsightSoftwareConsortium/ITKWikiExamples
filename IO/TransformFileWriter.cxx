#include "itkVersion.h"

#include "itkRigid2DTransform.h"
#include "itkTransformFileWriter.h"

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

  using TransformType = itk::Rigid2DTransform< float >;
  TransformType::Pointer transform = TransformType::New();

#if (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR >= 5) || ITK_VERSION_MAJOR > 4
  itk::TransformFileWriterTemplate<float>::Pointer writer =
    itk::TransformFileWriterTemplate<float>::New();
#else
  itk::TransformFileWriter::Pointer writer = itk::TransformFileWriter::New();
#endif

  writer->SetInput(transform);
  writer->SetFileName(fileName);
  writer->Update();

  return EXIT_SUCCESS;
}
