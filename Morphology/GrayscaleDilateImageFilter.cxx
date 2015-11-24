#include "itkImage.h"
#include "itkGrayscaleDilateImageFilter.h"
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

  unsigned int radius = 2;
  if (argc > 2)
    {
    radius = atoi(argv[2]);
    }

  std::string inputFilename = argv[1];

  typedef itk::Image<unsigned char, 2>    ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  typedef itk::BinaryBallStructuringElement<
    ImageType::PixelType,2> StructuringElementType;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  typedef itk::GrayscaleDilateImageFilter <ImageType, ImageType, StructuringElementType>
    GrayscaleDilateImageFilterType;

  GrayscaleDilateImageFilterType::Pointer dilateFilter
    = GrayscaleDilateImageFilterType::New();
  dilateFilter->SetInput(reader->GetOutput());
  dilateFilter->SetKernel(structuringElement);

  typedef itk::SubtractImageFilter<ImageType> SubtractType;
  SubtractType::Pointer diff = SubtractType::New();
  diff->SetInput2(reader->GetOutput());
  diff->SetInput1(dilateFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(inputFilename));  

  std::stringstream desc;
  desc << "GrayscaleDilate, radius = " << radius;
  viewer.AddImage(
    dilateFilter->GetOutput(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Original - GrayscaleDilate";
  viewer.AddImage(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
