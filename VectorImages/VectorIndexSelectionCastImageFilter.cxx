#include "itkVectorImage.h"
#include "itkImageRegionIterator.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkVectorIndexSelectionCastImageFilter.h"

using VectorImageType = itk::VectorImage<float, 2>;
using ScalarImageType = itk::Image<float, 2>;

static void CreateImage(VectorImageType::Pointer image);

int main(int, char *[])
{
  VectorImageType::Pointer image = VectorImageType::New();
  CreateImage(image);

  using IndexSelectionType = itk::VectorIndexSelectionCastImageFilter<VectorImageType, ScalarImageType>;
  IndexSelectionType::Pointer indexSelectionFilter = IndexSelectionType::New();
  indexSelectionFilter->SetIndex(0);
  indexSelectionFilter->SetInput(image);

  using ImageCalculatorFilterType = itk::MinimumMaximumImageCalculator <ScalarImageType>;
  ImageCalculatorFilterType::Pointer imageCalculatorFilter = ImageCalculatorFilterType::New();
  imageCalculatorFilter->SetImage(indexSelectionFilter->GetOutput());
  imageCalculatorFilter->Compute();

  return EXIT_SUCCESS;
}

void CreateImage(VectorImageType::Pointer image)
{
  VectorImageType::IndexType start;
  start.Fill(0);

  VectorImageType::SizeType size;
  size.Fill(2);

  VectorImageType::RegionType region(start, size);

  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(3);
  image->Allocate();

  using VariableVectorType = itk::VariableLengthVector<double>;
  VariableVectorType variableLengthVector;
  variableLengthVector.SetSize(3);
  variableLengthVector[0] = 1.1;
  variableLengthVector[1] = 2.2;
  variableLengthVector[2] = 3.3;

  image->FillBuffer(variableLengthVector);

}
