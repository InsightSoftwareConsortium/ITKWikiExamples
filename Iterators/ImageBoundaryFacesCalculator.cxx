#include "itkImage.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int, char*[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  ImageType::SizeType radius;
  radius.Fill(1);

  typedef itk::ImageRegionConstIterator<ImageType> IteratorType;

  typedef itk::NeighborhoodAlgorithm
    ::ImageBoundaryFacesCalculator< ImageType > FaceCalculatorType;

  FaceCalculatorType faceCalculator;
 
  FaceCalculatorType::FaceListType faceList;
  faceList = faceCalculator(image, image->GetLargestPossibleRegion(),
                            radius);
  
  FaceCalculatorType::FaceListType::iterator faceListIterator;

  for ( faceListIterator=faceList.begin(); faceListIterator != faceList.end(); ++faceListIterator)
    {
    IteratorType iterator(image,*faceListIterator);
    while(!iterator.IsAtEnd())
      {
      std::cout << iterator.GetIndex() << " ";
      ++iterator;
      }

    std::cout << std::endl;
    }

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = 5;
  size[1] = 5;

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(2);
    ++imageIterator;
    }

}
