#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkAbsImageFilter.h"

#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;
typedef itk::Image<float, 2>  FloatImageType;

static void CreateImage(FloatImageType::Pointer image);

int main(int, char *[])
{
  FloatImageType::Pointer image = FloatImageType::New();
  CreateImage(image);

  // Take the absolute value of the image
  typedef itk::AbsImageFilter <FloatImageType, FloatImageType>
          AbsImageFilterType;

  AbsImageFilterType::Pointer absFilter
          = AbsImageFilterType::New ();
  absFilter->SetInput(image);

  QuickView viewer;
  viewer.AddImage<FloatImageType>(image);
  viewer.AddImage<FloatImageType>(absFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateImage(FloatImageType::Pointer image)
{
  // Create an image with negative values
  FloatImageType::RegionType region;
  FloatImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  FloatImageType::SizeType size;
  size[0] = 200;
  size[1] = 300;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<FloatImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(imageIterator.GetIndex()[0] - imageIterator.GetIndex()[1]);
    ++imageIterator;
    }
  
}
