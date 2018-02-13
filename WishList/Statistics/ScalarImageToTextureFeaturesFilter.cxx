#include "itkImage.h"
#include "itkRandomImageSource.h"
#include "itkScalarImageToTextureFeaturesFilter.h"

using ImageType = itk::Image<float, 2>;

static void CreateImage(ImageType::Pointer);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  using TextureFilterType = itk::Statistics::ScalarImageToTextureFeaturesFilter<ImageType>;
  TextureFilterType::Pointer textureFilter = TextureFilterType::New();
  textureFilter->SetInput(image);
  textureFilter->Update();

  const TextureFilterType::FeatureValueVector* output = textureFilter->GetFeatureMeans();
  for(unsigned int i = 0; i < output->size(); ++i)
    {
    std::cout << (*output)[i] << std::endl;
    }
  
  return EXIT_SUCCESS;
}

static void CreateImage(ImageType::Pointer image)
{
  itk::Index<2> index;
  index.Fill(0);

  itk::Size<2> size;
  size.Fill(100);

  itk::ImageRegion<2> region(index, size);
  image->SetRegions(region);
  image->Allocate();
}
