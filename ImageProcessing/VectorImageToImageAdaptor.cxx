#include "itkVectorImage.h"
#include "itkImage.h"
#include "itkVectorImageToImageAdaptor.h"
#include "itkImageRegionIterator.h"

typedef itk::Image<float, 2> ScalarImageType;
typedef itk::VectorImage<float, 2> VectorImageType;

void CreateImage(VectorImageType::Pointer image);

int main(int, char *[])
{
  VectorImageType::Pointer image = VectorImageType::New();
  CreateImage(image);

  typedef itk::VectorImageToImageAdaptor<float, 2> ImageAdaptorType;
  ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();
  adaptor->SetExtractComponentIndex(0);
  adaptor->SetImage(image);

  itk::Index<2> index;
  index[0] = 0;
  index[1] = 0;
  
  std::cout << adaptor->GetPixel(index) << std::endl;

  return EXIT_SUCCESS;
}

void CreateImage(VectorImageType::Pointer image)
{
  VectorImageType::IndexType start;
  start.Fill(0);

  VectorImageType::SizeType size;
  size.Fill(2);

  VectorImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(2);
  image->Allocate();
  
  typedef itk::VariableLengthVector<double> VariableVectorType;
  VariableVectorType variableLengthVector;
  variableLengthVector.SetSize(2);

  itk::ImageRegionIterator<VectorImageType> imageIterator(image,image->GetLargestPossibleRegion());
  variableLengthVector[0] = 1;
  variableLengthVector[1] = 2;

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(variableLengthVector);

    ++imageIterator;
    }
}
