#include "itkSize.h"

#include "itkImage.h"
#include "itkImageFileReader.h"

int main(int, char *[])
{
  typedef itk::Image< double, 2 >         ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;

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
