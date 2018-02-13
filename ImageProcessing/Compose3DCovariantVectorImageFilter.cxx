#include "itkImageAdaptor.h"
#include "itkImageRegionIterator.h"
#if ITK_VERSION_MAJOR < 4
#include "itkCompose3DCovariantVectorImageFilter.h"
#else
#include "itkComposeImageFilter.h"
#endif

using VectorImageType = itk::Image<itk::CovariantVector< float, 3>, 2>;
using ScalarImageType = itk::Image<float, 2>;

static void CreateImage(ScalarImageType::Pointer image);

int main(int, char *[])
{
  ScalarImageType::Pointer image = ScalarImageType::New();
  CreateImage(image);

#if ITK_VERSION_MAJOR < 4
  using ComposeCovariantVectorImageFilterType = itk::Compose3DCovariantVectorImageFilter<ScalarImageType,
                              VectorImageType>;
#else
  using ComposeCovariantVectorImageFilterType = itk::ComposeImageFilter<ScalarImageType,
                              VectorImageType>;
#endif
  ComposeCovariantVectorImageFilterType::Pointer composeFilter = ComposeCovariantVectorImageFilterType::New();

  composeFilter->SetInput1(image);
  composeFilter->SetInput2(image);
  composeFilter->SetInput3(image);
  composeFilter->Update();
  
  itk::Index<2> index;
  index.Fill(0);

  std::cout << image->GetPixel(index) << std::endl;

  std::cout << composeFilter->GetOutput()->GetPixel(index) << std::endl;

  return EXIT_SUCCESS;
}

void CreateImage(ScalarImageType::Pointer image)
{
  ScalarImageType::IndexType start;
  start.Fill(0);

  ScalarImageType::SizeType size;
  size.Fill(2);

  ScalarImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ScalarImageType> imageIterator(image,image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    imageIterator.Set(1.2);

    ++imageIterator;
    }
}
