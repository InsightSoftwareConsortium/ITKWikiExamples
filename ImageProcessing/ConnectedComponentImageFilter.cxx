#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "QuickView.h"
 
typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main( int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  typedef itk::ConnectedComponentImageFilter <ImageType, ImageType >
    ConnectedComponentImageFilterType;

  ConnectedComponentImageFilterType::Pointer labelFilter
    = ConnectedComponentImageFilterType::New ();
  labelFilter->SetInput(image);
  labelFilter->Update();

  typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->SetInput(labelFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage<ImageType>( image, false );  
  viewer.AddImage<ImageType>( rescaleFilter->GetOutput(), false );  
  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create an image with 2 connected components
  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(unsigned int r = 20; r < 80; r++)
    {
    for(unsigned int c = 30; c < 100; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 255);
      }
    }

  // Make another square
  for(unsigned int r = 100; r < 130; r++)
    {
    for(unsigned int c = 115; c < 160; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 255);
      }
    }
}
