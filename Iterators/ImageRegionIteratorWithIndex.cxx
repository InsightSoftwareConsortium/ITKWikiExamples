#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIteratorWithIndex.h"

#include "QuickView.h"

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

  itk::ImageRegionIteratorWithIndex<ImageType> imageIterator(image,region);

  while(!imageIterator.IsAtEnd())
    {
    std::cout << "Index: " << imageIterator.GetIndex() << " value: " << imageIterator.Get() << std::endl;
  
    // Set the current pixel to white
    imageIterator.Set(255);
    
    ++imageIterator;
    }

  // Visualize
  QuickView viewer;
  viewer.AddImage<ImageType>( image );  
  viewer.Visualize();

  return EXIT_SUCCESS;
}
