#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "MultiThreadedImageFilter.h"

template <typename TImage>
static void CreateImage(TImage* const image);

int main(int, char*[])
{
  // Setup types
  typedef itk::Image<int, 2>   ImageType;
  typedef itk::MultiThreadedImageFilter<ImageType>  FilterType;

  ImageType::Pointer image = ImageType::New();
  CreateImage(image.GetPointer());

  // Create and the filter
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(image);
  filter->SetNumberOfThreads(3);
  filter->Update();

  itk::Index<2> cornerPixel = image->GetLargestPossibleRegion().GetIndex();

  // The output here is:
  // 0
  // 3
  // That is, the filter changed the pixel, but the input remained unchagned.
  std::cout << image->GetPixel(cornerPixel) << std::endl;
  std::cout << filter->GetOutput()->GetPixel(cornerPixel) << std::endl;

  return EXIT_SUCCESS;
}


template <typename TImage>
void CreateImage(TImage* const image)
{
  // Create an image with 2 connected components
  typename TImage::IndexType corner = {{0,0}};

  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  typename TImage::SizeType size = {{NumRows, NumCols}};

  typename TImage::RegionType region(corner, size);

  image->SetRegions(region);
  image->Allocate();

  image->FillBuffer(0);
}
