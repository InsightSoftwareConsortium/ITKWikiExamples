#include "itkImage.h"
#if ITK_VERSION_MAJOR >= 4
#include "itkMultiplyImageFilter.h"
#else
#include "itkMultiplyByConstantImageFilter.h"
#endif
#include "itkImageRegionConstIterator.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void OutputImage(ImageType::Pointer image);

int main( int argc, char *argv[])
{
  
  ImageType::Pointer image = ImageType::New();
  
  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size;
  size.Fill(3);

  itk::ImageRegion<2> region(start, size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(2);

  OutputImage(image);

#if ITK_VERSION_MAJOR >= 4
  typedef itk::MultiplyImageFilter<ImageType, ImageType, ImageType> MultiplyImageFilterType;
#else
  typedef itk::MultiplyByConstantImageFilter<ImageType, unsigned char, ImageType> MultiplyImageFilterType;
#endif
  MultiplyImageFilterType::Pointer multiplyImageFilter = MultiplyImageFilterType::New();
  multiplyImageFilter->SetInput(image);
#if ITK_VERSION_MAJOR >= 4
  multiplyImageFilter->SetConstant(3);
#else
  multiplyImageFilter->Update();
#endif

  OutputImage(multiplyImageFilter->GetOutput());
  
  return EXIT_SUCCESS;
}

void OutputImage(ImageType::Pointer image)
{
  itk::ImageRegionConstIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    // Get the value of the current pixel
    unsigned char val = imageIterator.Get();
    std::cout << (int)val << std::endl;

    ++imageIterator;
    }
}
