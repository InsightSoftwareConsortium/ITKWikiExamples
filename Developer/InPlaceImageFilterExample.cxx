#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "MyInPlaceImageFilter.h"

template <typename TImage>
static void CreateImage(TImage* const image);

int main(int, char*[])
{
  // Setup types
  typedef itk::Image<int, 2>   ImageType;
  typedef itk::MyInPlaceImageFilter<ImageType>  FilterType;

  ImageType::Pointer image = ImageType::New();
  CreateImage(image.GetPointer());

  // Create and the filter
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(image);
  filter->SetInPlace(true);
  filter->Update();

  itk::Index<2> cornerPixel = image->GetLargestPossibleRegion().GetIndex();

  itk::Index<2> cornerPixel2 = {{image->GetLargestPossibleRegion().GetSize()[0] - 1,
                                 image->GetLargestPossibleRegion().GetSize()[1] - 1}};

  // The output here is:
  // 3
  // 4
  std::cout << filter->GetOutput()->GetPixel(cornerPixel) << std::endl;
  std::cout << filter->GetOutput()->GetPixel(cornerPixel) << std::endl;

  // The output here is also:
  // 3
  // 4
  // That is, the filter changed the pixel in the output, and also in the input.
  std::cout << image->GetPixel(cornerPixel) << std::endl;
  std::cout << image->GetPixel(cornerPixel2) << std::endl;



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
