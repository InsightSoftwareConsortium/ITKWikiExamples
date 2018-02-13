#include "itkImage.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkImageFileReader.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkSubtractImageFilter.h"

#include "QuickView.h"

namespace
{
using ImageType = itk::Image<unsigned char, 2>;
}

static void CreateImage(ImageType* const image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image;

  if(argc == 1)
    {
    image = ImageType::New();
    CreateImage(image);
    }
  else
  {
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    image = reader->GetOutput();
  }

  unsigned int radius = 5;
  if (argc == 3)
    {
    std::stringstream ss(argv[2]);
    ss >> radius;
    }
  std::cout << "Radius: " << radius << std::endl;
  using StructuringElementType = itk::BinaryBallStructuringElement<ImageType::PixelType, ImageType::ImageDimension>;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  using BinaryMorphologicalOpeningImageFilterType = itk::BinaryMorphologicalOpeningImageFilter <ImageType, ImageType, StructuringElementType>;
  BinaryMorphologicalOpeningImageFilterType::Pointer openingFilter
          = BinaryMorphologicalOpeningImageFilterType::New();
  openingFilter->SetInput(image);
  openingFilter->SetKernel(structuringElement);
  openingFilter->Update();

  using SubtractType = itk::SubtractImageFilter<ImageType>;
  SubtractType::Pointer diff = SubtractType::New();
  diff->SetInput1(image);
  diff->SetInput2(openingFilter->GetOutput());

  QuickView viewer;
  std::stringstream desc;
  desc << "Original ";
  viewer.AddImage(image.GetPointer(),
                  true,
                  desc.str());

  std::stringstream desc2;
  desc2 << "BinaryOpening, radius = " << radius;
  viewer.AddImage(openingFilter->GetOutput(),
                  true,
                  desc2.str());

  std::stringstream desc3;
  desc3 << "Original - BinaryOpening";
  viewer.AddImage(diff->GetOutput(),
                  true,
                  desc3.str());
  viewer.Visualize();

  return EXIT_SUCCESS;
}


void CreateImage(ImageType* const image)
{
  // Create an image with 2 connected components
  itk::Index<2> corner = {{0,0}};

  itk::Size<2> size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  itk::ImageRegion<2> region(corner, size);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(unsigned int r = 40; r < 100; r++)
    {
    for(unsigned int c = 40; c < 100; c++)
      {
      itk::Index<2> pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 50);
      }
    }
}
