#include "itkSize.h"

#include "itkImage.h"
#include "itkImageFileReader.h"

int main(int, char *[])
{
  using ImageType = itk::Image< double, 2 >;
  using ReaderType = itk::ImageFileReader<ImageType>;

  try
  {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("nofile.png");
    reader->Update();
  }
  catch( itk::ExceptionObject & err )
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_SUCCESS; // Since the goal of the example is to catch the exception, we declare this a success.
  }

  return EXIT_FAILURE; // Since the goal of the example is to catch the exception, the example fails if it is not caught.
}
