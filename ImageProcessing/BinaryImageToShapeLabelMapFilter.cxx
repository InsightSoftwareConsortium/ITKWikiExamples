#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkBinaryImageToShapeLabelMapFilter.h"

typedef itk::Image<unsigned char, 2>  ImageType;
void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
 
  typedef itk::BinaryImageToShapeLabelMapFilter<ImageType> LabelMapFilterType;
  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(image);
  labelMapFilter->Update();
  LabelMapFilterType::OutputImageType * labelMap = labelMapFilter->GetOutput();
   
  std::cout << "There are " << labelMap->GetNumberOfLabelObjects() << " objects." << std::endl;
  for(unsigned int i = 0; i < labelMap->GetNumberOfLabelObjects(); i++)
    {
    std::cout << "Object " << i << " has bounding box " << labelMap->GetNthLabelObject(i)->GetBoundingBox() << std::endl;
    }
 
  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(20);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());

  // Make a square
  while(!imageIterator.IsAtEnd())
  {
    if((imageIterator.GetIndex()[0] > 5 && imageIterator.GetIndex()[0] < 10) &&
      (imageIterator.GetIndex()[1] > 5 && imageIterator.GetIndex()[1] < 10) )
        {
        imageIterator.Set(255);
        }
      else
        {
        imageIterator.Set(0);
        }

    ++imageIterator;
  }

}
