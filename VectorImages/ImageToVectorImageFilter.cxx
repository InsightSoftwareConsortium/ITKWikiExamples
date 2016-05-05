#include "itkImage.h"
#include "itkComposeImageFilter.h"
#include "itkVectorImage.h"

namespace
{
typedef itk::VectorImage<unsigned char, 2>  VectorImageType;
typedef itk::Image<unsigned char, 2>  ScalarImageType;
}

static void CreateImage(ScalarImageType::Pointer image);

int main(int, char *[])
{
  ScalarImageType::Pointer image0 = ScalarImageType::New();
  CreateImage(image0);
  
  ScalarImageType::Pointer image1 = ScalarImageType::New();
  CreateImage(image1);
  
  ScalarImageType::Pointer image2 = ScalarImageType::New();
  CreateImage(image2);
  
  typedef itk::ComposeImageFilter<ScalarImageType> ImageToVectorImageFilterType;
  ImageToVectorImageFilterType::Pointer imageToVectorImageFilter = ImageToVectorImageFilterType::New();
  imageToVectorImageFilter->SetInput(0, image0);
  imageToVectorImageFilter->SetInput(1, image1);
  imageToVectorImageFilter->SetInput(2, image2);
  imageToVectorImageFilter->Update();
  
  VectorImageType::Pointer vectorImage = imageToVectorImageFilter->GetOutput();
  
  return EXIT_SUCCESS;
}

void CreateImage(ScalarImageType::Pointer image)
{
  ScalarImageType::IndexType start;
  start.Fill(0);
 
  ScalarImageType::SizeType size;
  size.Fill(100);

  ScalarImageType::RegionType region(start,size); 
 
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
  
}
