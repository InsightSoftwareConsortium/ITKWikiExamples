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

  using MatrixOffsetTransformType = itk::MatrixOffsetTransformBase< double, 3, 3 >;
  itk::TransformFactory<MatrixOffsetTransformType>::RegisterTransform();

#if (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR >= 5) || ITK_VERSION_MAJOR > 4
  itk::TransformFileReaderTemplate<float>::Pointer reader =
    itk::TransformFileReaderTemplate<float>::New();
#else
  itk::TransformFileReader::Pointer reader = itk::TransformFileReader::New();
#endif
  reader->SetFileName(fileName);
  reader->Update();

  std::cout << *(reader->GetTransformList()->begin()) << std::endl;

  return EXIT_SUCCESS;
}
