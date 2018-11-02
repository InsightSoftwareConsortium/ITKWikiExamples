#include "itkImage.h"
#include "itkBinaryDilateImageFilter.h"
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
    radius = std::stoi(argv[2]);
    }

  using ImageType = itk::Image<unsigned char, 2>;
  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  using StructuringElementType = itk::BinaryBallStructuringElement<
    ImageType::PixelType,2>;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  using BinaryDilateImageFilterType = itk::BinaryDilateImageFilter <ImageType, ImageType, StructuringElementType>;

  BinaryDilateImageFilterType::Pointer dilateFilter
          = BinaryDilateImageFilterType::New();
  dilateFilter->SetInput(reader->GetOutput());
  dilateFilter->SetKernel(structuringElement);

  QuickView viewer;
  viewer.AddImage(reader->GetOutput());
  viewer.AddImage(dilateFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
