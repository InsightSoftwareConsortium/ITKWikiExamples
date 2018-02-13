#include "itkImage.h"
#include "itkImageFileReader.h"

int main( int argc, char * argv [] )
{
  if( argc < 4 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  x y " << std::endl;
    return EXIT_FAILURE;
    }

  using PixelType = short;
  constexpr unsigned int Dimension = 2;

  using ImageType = itk::Image< PixelType, Dimension >;
  using ReaderType = itk::ImageFileReader< ImageType >;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1]  );

  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  const ImageType * inputImage = reader->GetOutput();

  ImageType::IndexType index;

  index[0] = atoi( argv[2] );
  index[1] = atoi( argv[3] );

  const PixelType value = inputImage->GetPixel( index );

  std::cout << index << " = " << value << std::endl;

  return EXIT_SUCCESS;
}
