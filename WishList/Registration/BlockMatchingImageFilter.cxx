#include "itkBlockMatchingImageFilter.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkPoint.h"
#include "itkPointSet.h"

typedef itk::Image<unsigned char, 2> ImageType;

static void CreateImage(ImageType::Pointer image, const unsigned int x);

int main(int argc, char *argv[])
{
  // Create input images
  ImageType::Pointer fixedImage = ImageType::New();
  CreateImage(fixedImage, 40);

  ImageType::Pointer movingImage = ImageType::New();
  CreateImage(movingImage, 50);

//  typedef  itk::ImageFileWriter<ImageType> WriterType;
//  WriterType::Pointer writer = WriterType::New();
//  writer->SetFileName("input.png");
//  writer->SetInput(input);
//  writer->Update();

  //  typedef itk::BlockMatchingImageFilter<ImageType, ImageType, PointSetType> BlockMatchingImageFilterType;
  typedef itk::BlockMatchingImageFilter<ImageType> BlockMatchingImageFilterType;
  BlockMatchingImageFilterType::Pointer blockMatchingImageFilter =
        BlockMatchingImageFilterType::New();

  // Generate feature points
//  typedef itk::PointSet< float, 2>   PointSetType;
  typedef BlockMatchingImageFilterType::FeaturePointsType   PointSetType;
  typedef PointSetType::PointType PointType;
  typedef PointSetType::PointsContainerPointer PointsContainerPointer;

  PointSetType::Pointer   pointSet = PointSetType::New();
  PointsContainerPointer  points = pointSet->GetPoints();

  PointType p0, p1, p2, p3;

  p0[0]=  40.0; p0[1]= 40.0;
  p1[0]=  40.0; p1[1]= 60.0;
  p2[0]=  60.0; p2[1]= 40.0;
  p3[0]=  60.0; p2[1]= 60.0;

  points->InsertElement(0, p0);
  points->InsertElement(1, p1);
  points->InsertElement(2, p2);
  points->InsertElement(3, p3);

  blockMatchingImageFilter->SetFixedImage(fixedImage);
  blockMatchingImageFilter->SetMovingImage(movingImage);
  blockMatchingImageFilter->SetFeaturePoints(pointSet);
  blockMatchingImageFilter->UpdateLargestPossibleRegion();

  typename BlockMatchingImageFilterType::DisplacementsType * displacements =
          blockMatchingImageFilter->GetDisplacements();

  std::cout << "There are " << displacements->GetNumberOfPoints() << " displacements." << std::endl;

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image, const unsigned int x)
{
  // Allocate empty image
  itk::Index<2> start; start.Fill(0);
  itk::Size<2> size; size.Fill(100);
  ImageType::RegionType region(start, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // Make a white square
  for(unsigned int r = x; r < x+20; r++)
    {
    for(unsigned int c = 40; c < 60; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;
      image->SetPixel(pixelIndex, 255);
      }
    }
}
