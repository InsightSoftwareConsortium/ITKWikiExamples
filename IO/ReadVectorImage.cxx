#include "itkVectorImage.h"
#include "itkImageFileReader.h"

int main(int argc, char *argv[])
{
  if (argc < 2)
    {
    std::cerr << "Usage: " << argv[0] << " imageFilename" << std::endl;
    return EXIT_FAILURE;
    }
  std::string filename = argv[1];
  
  typedef itk::VectorImage<float, 2>  ImageType;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->Update();

  std::cout << reader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
  
  return EXIT_SUCCESS;
}
