#include "itkImage.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryCrossStructuringElement.h"

namespace
{
using ImageType = itk::Image<int, 2>;
using PixelType = ImageType::PixelType;
}
 
static void CreateImage(ImageType::Pointer image);
 
int main(int, char*[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
   
  using StructuringElementType = itk::BinaryBallStructuringElement<PixelType, 2>;
  StructuringElementType::RadiusType elementRadius;
  elementRadius.Fill(2);
  
  StructuringElementType structuringElement;
  structuringElement.SetRadius(elementRadius);
  structuringElement.CreateStructuringElement();

  using IteratorType = itk::ShapedNeighborhoodIterator<ImageType>;
  IteratorType siterator(structuringElement.GetRadius(),
                         image,
                         image->GetLargestPossibleRegion());
  
  siterator.CreateActiveListFromNeighborhood(structuringElement);
  siterator.NeedToUseBoundaryConditionOff();

  IteratorType::IndexType location;
  location[0] = 4;
  location[1] = 5;
  siterator.SetLocation(location);
  IteratorType::Iterator i;
  for (i = siterator.Begin(); !i.IsAtEnd(); ++i)
    {
    i.Set(1);
    }
  
  // Now show the results
  using ImageIteratorType = itk::ImageRegionConstIterator<ImageType>;
  ImageIteratorType imit(image, image->GetLargestPossibleRegion());
  imit.GoToBegin();
  unsigned int col = 0;
  while( !imit.IsAtEnd() )
    {
    PixelType value = imit.Get();
    ++imit;
    ++col;
    std::cout << value << " ";
    if ((col % 10) == 0)
      {
      std::cout << std::endl;
      }
    }

  return EXIT_SUCCESS;
}
 
void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);
 
  ImageType::SizeType size;
  size.Fill(10);
  
  ImageType::RegionType region(start,size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
}
