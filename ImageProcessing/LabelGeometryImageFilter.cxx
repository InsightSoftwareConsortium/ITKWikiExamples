#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkLabelGeometryImageFilter.h"

typedef itk::Image<unsigned char, 2>  ImageType;
static void CreateIntensityImage(ImageType::Pointer image);
static void CreateLabelImage(ImageType::Pointer image);

int main(int, char *[])
{
  // Create a label image that is 0 in the background and where the objects are labeled
  ImageType::Pointer labelImage = ImageType::New();
  CreateLabelImage(labelImage);

  // Create an intensity image.  Some LabelGeometry features (such as weighted centroid and integrated intensity)
  // depend on an intensity image.
  ImageType::Pointer intensityImage = ImageType::New();
  CreateIntensityImage(intensityImage);

  typedef itk::LabelGeometryImageFilter< ImageType > LabelGeometryImageFilterType;
  LabelGeometryImageFilterType::Pointer labelGeometryImageFilter = LabelGeometryImageFilterType::New();
  labelGeometryImageFilter->SetInput( labelImage );
  labelGeometryImageFilter->SetIntensityInput( intensityImage );

  // These generate optional outputs.
  labelGeometryImageFilter->CalculatePixelIndicesOn();
  labelGeometryImageFilter->CalculateOrientedBoundingBoxOn();
  labelGeometryImageFilter->CalculateOrientedLabelRegionsOn();
  
  labelGeometryImageFilter->Update();
  
  LabelGeometryImageFilterType::LabelsType allLabels = labelGeometryImageFilter->GetLabels();
  LabelGeometryImageFilterType::LabelsType::iterator allLabelsIt;
  std::cout << "Number of labels: " << labelGeometryImageFilter->GetNumberOfLabels() << std::endl;
  std::cout << std::endl;

  for( allLabelsIt = allLabels.begin(); allLabelsIt != allLabels.end(); allLabelsIt++ )
    {
    LabelGeometryImageFilterType::LabelPixelType labelValue = *allLabelsIt;
    std::cout << "\tLabel: " << (int)labelValue << std::endl;
    std::cout << "\tVolume: " << labelGeometryImageFilter->GetVolume(labelValue) << std::endl;
    std::cout << "\tIntegrated Intensity: " << labelGeometryImageFilter->GetIntegratedIntensity(labelValue) << std::endl;
    std::cout << "\tCentroid: " << labelGeometryImageFilter->GetCentroid(labelValue) << std::endl;
    std::cout << "\tWeighted Centroid: " << labelGeometryImageFilter->GetWeightedCentroid(labelValue) << std::endl;
    std::cout << "\tAxes Length: " << labelGeometryImageFilter->GetAxesLength(labelValue) << std::endl;
    std::cout << "\tMajorAxisLength: " << labelGeometryImageFilter->GetMajorAxisLength(labelValue) << std::endl;
    std::cout << "\tMinorAxisLength: " << labelGeometryImageFilter->GetMinorAxisLength(labelValue) << std::endl;
    std::cout << "\tEccentricity: " << labelGeometryImageFilter->GetEccentricity(labelValue) << std::endl;
    std::cout << "\tElongation: " << labelGeometryImageFilter->GetElongation(labelValue) << std::endl;
    std::cout << "\tOrientation: " << labelGeometryImageFilter->GetOrientation(labelValue) << std::endl;
    std::cout << "\tBounding box: " << labelGeometryImageFilter->GetBoundingBox(labelValue) << std::endl;
    
    std::cout << std::endl << std::endl;
    }

  return EXIT_SUCCESS;
}

void CreateIntensityImage(ImageType::Pointer image)
{
  // Create a random image.
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
  // Make a random image
  // Create an unchanging seed.
  srand(1);
  while(!imageIterator.IsAtEnd())
    {
    int randomNumber = rand() % 255;
    imageIterator.Set( randomNumber );
    ++imageIterator;
    }

  typedef itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("intensityImage.png");
  writer->SetInput(image);
  writer->Update();
}

void CreateLabelImage(ImageType::Pointer image)
{
  // Create a black image with labeled squares.
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
        imageIterator.Set(85);
        }
    else if((imageIterator.GetIndex()[0] > 11 && imageIterator.GetIndex()[0] < 17) &&
      (imageIterator.GetIndex()[1] > 11 && imageIterator.GetIndex()[1] < 17) )
	{
	imageIterator.Set(127);
	}
    else if((imageIterator.GetIndex()[0] > 11 && imageIterator.GetIndex()[0] < 17) &&
      (imageIterator.GetIndex()[1] > 1 && imageIterator.GetIndex()[1] < 5) )
	{
	imageIterator.Set(191);
	}
    else
        {
        imageIterator.Set(0);
        }

    ++imageIterator;
    }

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("image.png");
  writer->SetInput(image);
  writer->Update();
}
