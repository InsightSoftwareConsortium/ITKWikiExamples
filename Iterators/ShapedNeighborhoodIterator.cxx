#include "itkImage.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
//#include "itkFlatStructuringElement.h"
#include "itkBinaryBallStructuringElement.h"

typedef itk::Image<int, 2>  ImageType;
 
static void CreateImage(ImageType::Pointer image);
 
int main(int, char*[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  ImageType::SizeType radius;
  radius.Fill(1);
   
  //typedef itk::FlatStructuringElement<2> StructuringElementType;
  typedef itk::BinaryBallStructuringElement<int, 2> StructuringElementType;
  StructuringElementType::RadiusType elementRadius;
  elementRadius.Fill(3);
  
  StructuringElementType structuringElement;
  structuringElement.SetRadius(elementRadius);
  structuringElement.CreateStructuringElement();
  
  typedef itk::ShapedNeighborhoodIterator<ImageType> IteratorType;
  IteratorType iterator(structuringElement.GetRadius(), image, image->GetLargestPossibleRegion());
  
  iterator.SetNeighborhood(structuringElement);
  
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
