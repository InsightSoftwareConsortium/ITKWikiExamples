#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageRegionExclusionConstIteratorWithIndex.h"

namespace
{
using ImageType = itk::Image<int, 2>;
}

static void CreateImage(ImageType::Pointer);

int main(int, char*[])
{

  ImageType::SizeType exclusionRegionSize;
  exclusionRegionSize.Fill(2);

  ImageType::IndexType exclusionRegionIndex;
  exclusionRegionIndex.Fill(0);

  ImageType::RegionType exclusionRegion(exclusionRegionIndex, exclusionRegionSize);

  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  
  itk::ImageRegionExclusionConstIteratorWithIndex<ImageType> imageIterator(image, image->GetLargestPossibleRegion());
  imageIterator.SetExclusionRegion(exclusionRegion);

  unsigned int numberVisited = 0;
  while(!imageIterator.IsAtEnd())
    {
    std::cout << imageIterator.Get() << std::endl;
    ++imageIterator;
    ++numberVisited;
    }
    
  std::cout << "Visited " << numberVisited << std::endl;
  
  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  ImageType::SizeType regionSize;
  regionSize.Fill(3);

  ImageType::IndexType regionIndex;
  regionIndex.Fill(0);

  ImageType::RegionType region(regionIndex,regionSize);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
  
}
