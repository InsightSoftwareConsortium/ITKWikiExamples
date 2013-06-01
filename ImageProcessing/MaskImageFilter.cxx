#include "itkConfigure.h"

#if ( ITK_VERSION_MAJOR < 4  ) //These are all defaults in ITKv4
//  Not supported in ITKv3.
int main(int argc, char *argv[])
{
  return 0;
}
#else
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMaskImageFilter.h"
#include "itkImageRegionIterator.h"
#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  ImageType;

void CreateHalfMask(ImageType::Pointer image, ImageType::Pointer &mask);

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  ImageType::Pointer mask = ImageType::New();
  CreateHalfMask(reader->GetOutput(), mask);
  
  typedef itk::MaskImageFilter< ImageType, ImageType > MaskFilterType;
  MaskFilterType::Pointer maskFilter = MaskFilterType::New();
  maskFilter->SetInput(reader->GetOutput());
  maskFilter->SetMaskImage(mask);
  mask->Print(std::cout);
  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "Mask";
  viewer.AddImage(
    mask.GetPointer(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "MaskFilter";
  viewer.AddImage(
    maskFilter->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}


void CreateHalfMask(ImageType::Pointer image, ImageType::Pointer &mask)
{
  ImageType::RegionType region = image->GetLargestPossibleRegion();
  
  mask->SetRegions(region);
  mask->Allocate();

  ImageType::SizeType regionSize = region.GetSize();

  itk::ImageRegionIterator<ImageType> imageIterator(mask,region);

  // Make the left half of the mask white and the right half black
  while(!imageIterator.IsAtEnd())
  {
     if(imageIterator.GetIndex()[0] > static_cast<ImageType::IndexValueType>(regionSize[0]) / 2)
        {
        imageIterator.Set(0);
        }
      else
        {
        imageIterator.Set(255);
        }

    ++imageIterator;
  }

}
#endif
