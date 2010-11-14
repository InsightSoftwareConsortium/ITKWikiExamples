#include "itkImage.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"

typedef itk::Image<unsigned char, 2>  ImageType;

void CreateImage(ImageType::Pointer image);

int main(int, char*[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  ImageType::SizeType radius;
  radius[0] = 1;
  radius[1] = 1;

  typedef itk::ShapedNeighborhoodIterator<ImageType> IteratorType;

  IteratorType::OffsetType top = {{0,-1}};
  IteratorType::OffsetType bottom = {{0,1}};
  IteratorType::OffsetType left = {{-1,0}};
  IteratorType::OffsetType right = {{1,0}};
  IteratorType::OffsetType center = {{0,0}};

  

  typedef itk::NeighborhoodAlgorithm
    ::ImageBoundaryFacesCalculator< ImageType > FaceCalculatorType;

  FaceCalculatorType faceCalculator;
  FaceCalculatorType::FaceListType faceList;
  faceList = faceCalculator(image, image->GetLargestPossibleRegion(),
                            radius);
  FaceCalculatorType::FaceListType::iterator faceListIterator;

  for ( faceListIterator=faceList.begin(); faceListIterator != faceList.end(); ++faceListIterator)
    {
    IteratorType iterator(radius, image, *faceListIterator);
    iterator.ActivateOffset(top);
    iterator.ActivateOffset(bottom);
    iterator.ActivateOffset(left);
    iterator.ActivateOffset(right);
    iterator.ActivateOffset(center);

    for(iterator.GoToBegin(); !iterator.IsAtEnd(); ++iterator) // Crashes here!
      {
      std::cout << "top: " << iterator[top] << std::endl;
      std::cout << "bottom: " << iterator[bottom] << std::endl;
      std::cout << "left: " << iterator[left] << std::endl;
      std::cout << "right: " << iterator[right] << std::endl;
      }
    }

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
  size[0] = 100;
  size[1] = 200;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(0);

    ++imageIterator;
    }

}
