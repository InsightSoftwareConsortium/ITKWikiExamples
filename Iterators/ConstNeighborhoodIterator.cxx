#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConstNeighborhoodIterator.h"

int main(int argc, char*argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  using ImageType = itk::Image<unsigned char, 2>;

  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  ImageType::Pointer image = reader->GetOutput();
  
  ImageType::SizeType regionSize;
  regionSize[0] = 50;
  regionSize[1] = 1;

  ImageType::IndexType regionIndex;
  regionIndex[0] = 0;
  regionIndex[1] = 0;

  ImageType::RegionType region;
  region.SetSize(regionSize);
  region.SetIndex(regionIndex);

  ImageType::SizeType radius;
  radius[0] = 1;
  radius[1] = 1;

  itk::ConstNeighborhoodIterator<ImageType> iterator(radius, image,region);

  
  while(!iterator.IsAtEnd())
    {
    for(unsigned int i = 0; i < 9; i++)
      {
      ImageType::IndexType index = iterator.GetIndex(i);
      std::cout << index[0] << " " << index[1] << std::endl;

      bool IsInBounds;
      iterator.GetPixel(i, IsInBounds);

      }
    ++iterator;
    }


  return EXIT_SUCCESS;
}
