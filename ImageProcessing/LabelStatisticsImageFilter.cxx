#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"

using ImageType = itk::Image<unsigned char, 2>;
static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  using BinaryImageToLabelMapFilterType = itk::BinaryImageToLabelMapFilter<ImageType>;
  BinaryImageToLabelMapFilterType::Pointer binaryImageToLabelMapFilter = BinaryImageToLabelMapFilterType::New();
  binaryImageToLabelMapFilter->SetInput(image);
  binaryImageToLabelMapFilter->Update();

  using LabelMapToLabelImageFilterType = itk::LabelMapToLabelImageFilter<BinaryImageToLabelMapFilterType::OutputImageType, ImageType>;
  LabelMapToLabelImageFilterType::Pointer labelMapToLabelImageFilter = LabelMapToLabelImageFilterType::New();
  labelMapToLabelImageFilter->SetInput(binaryImageToLabelMapFilter->GetOutput());
  labelMapToLabelImageFilter->Update();

  using LabelStatisticsImageFilterType = itk::LabelStatisticsImageFilter< ImageType, ImageType >;
  LabelStatisticsImageFilterType::Pointer labelStatisticsImageFilter = LabelStatisticsImageFilterType::New();
  labelStatisticsImageFilter->SetLabelInput( labelMapToLabelImageFilter->GetOutput() );
  labelStatisticsImageFilter->SetInput(image);
  labelStatisticsImageFilter->Update();
    
  std::cout << "Number of labels: " << labelStatisticsImageFilter->GetNumberOfLabels() << std::endl;
  std::cout << std::endl;
  
  using LabelPixelType = LabelStatisticsImageFilterType::LabelPixelType;

  for(auto vIt=labelStatisticsImageFilter->GetValidLabelValues().begin();
      vIt != labelStatisticsImageFilter->GetValidLabelValues().end();
      ++vIt)
    {
    if ( labelStatisticsImageFilter->HasLabel(*vIt) )
      {
      LabelPixelType labelValue = *vIt;
      std::cout << "min: " << labelStatisticsImageFilter->GetMinimum( labelValue ) << std::endl;
      std::cout << "max: " << labelStatisticsImageFilter->GetMaximum( labelValue ) << std::endl;
      std::cout << "median: " << labelStatisticsImageFilter->GetMedian( labelValue ) << std::endl;
      std::cout << "mean: " << labelStatisticsImageFilter->GetMean( labelValue ) << std::endl;
      std::cout << "sigma: " << labelStatisticsImageFilter->GetSigma( labelValue ) << std::endl;
      std::cout << "variance: " << labelStatisticsImageFilter->GetVariance( labelValue ) << std::endl;
      std::cout << "sum: " << labelStatisticsImageFilter->GetSum( labelValue ) << std::endl;
      std::cout << "count: " << labelStatisticsImageFilter->GetCount( labelValue ) << std::endl;
      //std::cout << "box: " << labelStatisticsImageFilter->GetBoundingBox( labelValue ) << std::endl; // can't output a box
      std::cout << "region: " << labelStatisticsImageFilter->GetRegion( labelValue ) << std::endl;
      std::cout << std::endl << std::endl;

      }
    }

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create a black image with a white square
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

  using WriterType = itk::ImageFileWriter< ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("image.png");
  writer->SetInput(image);
  writer->Update();
}
