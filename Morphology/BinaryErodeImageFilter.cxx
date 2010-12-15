#include "itkImage.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkImageFileReader.h"
#include "itkBinaryBallStructuringElement.h"

#include "QuickView.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImageFile [radius]" << std::endl;
    return EXIT_FAILURE;
    }

  unsigned int radius = 2;
  if (argc > 2)
    {
    radius = atoi(argv[2]);
    }

  typedef itk::Image<unsigned char, 2>    ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::BinaryBallStructuringElement<
    ImageType::PixelType, 2>                  StructuringElementType;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  typedef itk::BinaryErodeImageFilter <ImageType, ImageType, StructuringElementType>
    BinaryErodeImageFilterType;

  BinaryErodeImageFilterType::Pointer erodeFilter
    = BinaryErodeImageFilterType::New();
  erodeFilter->SetInput(reader->GetOutput());
  erodeFilter->SetKernel(structuringElement);

  QuickView viewer;
  viewer.AddImage(reader->GetOutput());
  viewer.AddImage(erodeFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
