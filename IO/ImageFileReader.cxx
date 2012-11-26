#include "itkImage.h"
#include "itkImageFileReader.h"

#include "QuickView.h"

int main(int argc, char *argv[])
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::Image< double, 2 >         ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  QuickView viewer;
  viewer.AddImage<ImageType>(reader->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
