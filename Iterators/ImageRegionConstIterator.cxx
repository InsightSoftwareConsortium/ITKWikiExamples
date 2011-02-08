#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageRegionConstIterator.h"

int main(int argc, char*argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::Image<unsigned char, 2>  ImageType;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  ImageType::Pointer image = reader->GetOutput();
  
  ImageType::SizeType regionSize;
  regionSize[0] = 5;
  regionSize[1] = 4;

  ImageType::IndexType regionIndex;
  regionIndex[0] = 0;
  regionIndex[1] = 0;

  ImageType::RegionType region;
  region.SetSize(regionSize);
  region.SetIndex(regionIndex);

  itk::ImageRegionConstIterator<ImageType> imageIterator(image,region);
  //itk::ImageRegionConstIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());

  while(!imageIterator.IsAtEnd())
    {
    // Get the value of the current pixel
    unsigned char val = imageIterator.Get();
    std::cout << (int)val << std::endl;

    ++imageIterator;
    }

  return EXIT_SUCCESS;
}
