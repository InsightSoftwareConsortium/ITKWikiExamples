#include "itkTileImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

int main(int argc, char *argv[] )
{

  using PixelType = unsigned char;
  const unsigned int InputImageDimension = 2;
  const unsigned int OutputImageDimension = 3;

  using InputImageType = itk::Image< PixelType, InputImageDimension  >;
  using OutputImageType = itk::Image< PixelType, OutputImageDimension >;

  using ImageReaderType = itk::ImageFileReader< InputImageType >;

  using TilerType = itk::TileImageFilter< InputImageType, OutputImageType >;

  using WriterType = itk::ImageFileWriter< OutputImageType >;

  if (argc < 4)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "input1 input2 ... inputn output" << std::endl;
    return EXIT_FAILURE;
    }

  TilerType::Pointer tiler = TilerType::New();

  itk::FixedArray< unsigned int, OutputImageDimension > layout;

  layout[0] = 1;
  layout[1] = 1;
  layout[2] = 0;

  tiler->SetLayout( layout );

  unsigned int inputImageNumber = 0;

  ImageReaderType::Pointer reader = ImageReaderType::New();

  InputImageType::Pointer inputImageTile;

  for (int i = 1; i < argc - 1; i++)
    {
    reader->SetFileName( argv[i] );
    reader->UpdateLargestPossibleRegion();
    inputImageTile = reader->GetOutput();
    inputImageTile->DisconnectPipeline();
    tiler->SetInput( inputImageNumber++, inputImageTile );
    }

  PixelType filler = 128;

  tiler->SetDefaultPixelValue( filler );

  tiler->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( tiler->GetOutput() );
  writer->SetFileName( argv[argc-1] );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
