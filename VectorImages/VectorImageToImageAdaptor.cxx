#include "itkVectorImage.h"
#include "itkImage.h"
#include "itkVectorImageToImageAdaptor.h"
#include "itkImageRegionIterator.h"

using ScalarImageType = itk::Image<float, 2>;
using VectorImageType = itk::VectorImage<float, 2>;

void CreateImage(VectorImageType::Pointer image);

int main(int, char *[])
{
  VectorImageType::Pointer image = VectorImageType::New();
  CreateImage(image);

  using ImageAdaptorType = itk::VectorImageToImageAdaptor<float, 2>;
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
  
  using VariableVectorType = itk::VariableLengthVector<double>;
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
