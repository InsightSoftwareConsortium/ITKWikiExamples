#include "itkImage.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkImageFileReader.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkImageFileWriter.h"

#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType* const image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image;

  if(argc == 1)
    {
//     std::cerr << "Usage: " << std::endl;
//     std::cerr << argv[0] << " InputImageFile OutputImageFile [radius]" << std::endl;
    image = ImageType::New();
    CreateImage(image);
    }
  else
  {
    typedef itk::ImageFileReader<ImageType> ReaderType;
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

  typedef itk::BinaryBallStructuringElement<ImageType::PixelType, ImageType::ImageDimension>
              StructuringElementType;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  typedef itk::BinaryMorphologicalOpeningImageFilter <ImageType, ImageType, StructuringElementType>
          BinaryMorphologicalOpeningImageFilterType;
  BinaryMorphologicalOpeningImageFilterType::Pointer openingFilter
          = BinaryMorphologicalOpeningImageFilterType::New();
  openingFilter->SetInput(image);
  openingFilter->SetKernel(structuringElement);
  openingFilter->Update();

//   typedef itk::ImageFileWriter< ImageType > WriterType;
//   WriterType::Pointer writer = WriterType::New();
//   writer->SetInput( dilateFilter->GetOutput() );
//   writer->SetFileName( argv[2] );
//   writer->Update();

  QuickView viewer;
  viewer.AddImage(image.GetPointer());
  viewer.AddImage(openingFilter->GetOutput());
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
