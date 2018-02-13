#include "itkImageAdaptor.h"
#include "itkImageRegionIterator.h"
#include "itkNthElementImageAdaptor.h"

using VectorImageType = itk::Image<itk::CovariantVector< float, 3>, 2>;

static void CreateImage(VectorImageType::Pointer image);

int main(int, char *[])
{
  VectorImageType::Pointer image = VectorImageType::New();
  CreateImage(image);

  itk::Index<2> index;
  index.Fill(0);

  std::cout << image->GetPixel(index) << std::endl;

  using ImageAdaptorType = itk::NthElementImageAdaptor<VectorImageType,
    float>;

  ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();

  adaptor->SelectNthElement(0);
  adaptor->SetImage(image);

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
  image->Allocate();

  // Create a pixel and fill it with (1,2,3), and set every pixel of the image to this pixel.

  itk::ImageRegionIterator<VectorImageType> imageIterator(image,image->GetLargestPossibleRegion());
  itk::CovariantVector<float, 3> vectorPixel;
  vectorPixel[0] = 1;
  vectorPixel[1] = 2;
  vectorPixel[2] = 3;

  image->FillBuffer(vectorPixel);
}
