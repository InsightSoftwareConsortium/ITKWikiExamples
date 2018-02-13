#include "itkImage.h"
#include "itkGrayscaleErodeImageFilter.h"
#include "itkImageFileReader.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkSubtractImageFilter.h"

#include "itksys/SystemTools.hxx"

#include "QuickView.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImageFile [radius]" << std::endl;
    return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];

  using ImageType = itk::Image<unsigned char, 2>;
  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  unsigned int radius = 2;
  if (argc > 2)
    {
    radius = atoi(argv[2]);
    }

  using StructuringElementType = itk::BinaryBallStructuringElement<
    ImageType::PixelType,
    2>;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  using GrayscaleErodeImageFilterType = itk::GrayscaleErodeImageFilter <ImageType, ImageType, StructuringElementType>;

  GrayscaleErodeImageFilterType::Pointer erodeFilter
    = GrayscaleErodeImageFilterType::New();
  erodeFilter->SetInput(reader->GetOutput());
  erodeFilter->SetKernel(structuringElement);

  using SubtractType = itk::SubtractImageFilter<ImageType>;
  SubtractType::Pointer diff = SubtractType::New();
  diff->SetInput1(reader->GetOutput());
  diff->SetInput2(erodeFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(inputFilename));  

  std::stringstream desc;
  desc << "GrayscaleErode, radius = " << radius;
  viewer.AddImage(
    erodeFilter->GetOutput(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Original - GrayscaleErode";
  viewer.AddImage(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
