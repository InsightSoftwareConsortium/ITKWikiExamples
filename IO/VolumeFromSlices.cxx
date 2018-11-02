#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"

int main( int argc, char * argv[] )
{
  // Verify the number of parameters in the command line
  if( argc < 5 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] 
              << " pattern firstSliceValue lastSliceValue outputImageFile"
              << std::endl;
    return EXIT_FAILURE;
    }

  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 3;

  using ImageType = itk::Image< PixelType, Dimension >;
  using ReaderType = itk::ImageSeriesReader< ImageType >;
  using WriterType = itk::ImageFileWriter<   ImageType >;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const unsigned int first = std::stoi( argv[2] );
  const unsigned int last  = std::stoi( argv[3] );

  const char * outputFilename = argv[4];

  using NameGeneratorType = itk::NumericSeriesFileNames;

  NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();

  nameGenerator->SetSeriesFormat( argv[1] );

  nameGenerator->SetStartIndex( first );
  nameGenerator->SetEndIndex( last );
  nameGenerator->SetIncrementIndex( 1 );
  std::vector<std::string> names = nameGenerator->GetFileNames();

  // List the files
  //
  std::vector<std::string>::iterator nit;
  for (nit = names.begin();
       nit != names.end();
       nit++)
    {
    std::cout << "File: " << (*nit).c_str() << std::endl;
    }
  
  reader->SetFileNames( names  );

  writer->SetFileName( outputFilename );
  writer->SetInput( reader->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
 }
