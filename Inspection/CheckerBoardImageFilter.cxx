#include "itkImage.h"
#include "itkRGBPixel.h"
#include "itkCheckerBoardImageFilter.h"
#include "itkImageRegionIterator.h"

#include "QuickView.h"

typedef itk::RGBPixel<unsigned char> PixelType;
typedef itk::Image<PixelType, 2> ImageType;

static void CreateImage(ImageType::Pointer image, PixelType);

int main(int argc, char *argv[])
{
  ImageType::Pointer image1 = ImageType::New();
  PixelType pixel1;
  pixel1.Fill(0);
  if (argc > 3)
    {
    pixel1.SetRed(atoi(argv[1]));
    pixel1.SetGreen(atoi(argv[2]));
    pixel1.SetBlue(atoi(argv[3]));
    }
  CreateImage(image1, pixel1);
  ImageType::Pointer image2 = ImageType::New();

  PixelType pixel2;
  pixel2.Fill(255);
  if (argc > 6)
    {
    pixel2.SetRed(atoi(argv[4]));
    pixel2.SetGreen(atoi(argv[5]));
    pixel2.SetBlue(atoi(argv[6]));
    }
  CreateImage(image2, pixel2);
  
  typedef itk::CheckerBoardImageFilter< ImageType > CheckerBoardFilterType;
  CheckerBoardFilterType::Pointer checkerBoardFilter = CheckerBoardFilterType::New();
  checkerBoardFilter->SetInput1(image1);
  checkerBoardFilter->SetInput2(image2);
  checkerBoardFilter->Update();
  
  QuickView viewer;
  viewer.AddImage<ImageType>(image1);
  viewer.AddImage<ImageType>(image2);
  viewer.AddImage<ImageType>(checkerBoardFilter->GetOutput());
  viewer.Visualize();
 
  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image, PixelType color)
{
  // Create an image with all pixels set to 'color'
  
  ImageType::IndexType start;
  start.Fill(0);
  
  ImageType::SizeType size;
  size.Fill(100);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();
  
  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(color);

    ++imageIterator;
    }
}
