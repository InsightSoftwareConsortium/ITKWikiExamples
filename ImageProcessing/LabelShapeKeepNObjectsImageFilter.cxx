#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelShapeKeepNObjectsImageFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"

using ImageType = itk::Image<unsigned char, 2>;
using LabelImageType = itk::Image<unsigned char, 2>;
static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  using BinaryImageToLabelMapFilterType = itk::BinaryImageToLabelMapFilter<ImageType>;
  BinaryImageToLabelMapFilterType::Pointer binaryImageToLabelMapFilter = BinaryImageToLabelMapFilterType::New();
  binaryImageToLabelMapFilter->SetInput(image);
  binaryImageToLabelMapFilter->Update();
  
  using LabelMapToLabelImageFilterType = itk::LabelMapToLabelImageFilter<BinaryImageToLabelMapFilterType::OutputImageType, LabelImageType>;
  LabelMapToLabelImageFilterType::Pointer labelMapToLabelImageFilter = LabelMapToLabelImageFilterType::New();
  labelMapToLabelImageFilter->SetInput(binaryImageToLabelMapFilter->GetOutput());
  labelMapToLabelImageFilter->Update();

  using LabelShapeKeepNObjectsImageFilterType = itk::LabelShapeKeepNObjectsImageFilter< LabelImageType >;
  LabelShapeKeepNObjectsImageFilterType::Pointer labelShapeKeepNObjectsImageFilter = LabelShapeKeepNObjectsImageFilterType::New();
  labelShapeKeepNObjectsImageFilter->SetInput( labelMapToLabelImageFilter->GetOutput() );
  labelShapeKeepNObjectsImageFilter->SetBackgroundValue( 0 );
  labelShapeKeepNObjectsImageFilter->SetNumberOfObjects( 2 );
  //KeepNObjects->ReverseOrderingOn();
  //labelShapeKeepNObjectsImageFilter->SetAttribute( LabelObjectType::PERIMETER );
  labelShapeKeepNObjectsImageFilter->SetAttribute( LabelShapeKeepNObjectsImageFilterType::LabelObjectType::PERIMETER);
  labelShapeKeepNObjectsImageFilter->Update();
 
  using RGBPixelType = itk::RGBPixel<unsigned char>;
  using RGBImageType = itk::Image<RGBPixelType, 2>;

  // Color each label/object a different color
  using RGBFilterType = itk::ScalarToRGBColormapImageFilter<LabelImageType, RGBImageType>;
  RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
  colormapImageFilter->SetInput(labelShapeKeepNObjectsImageFilter->GetOutput());
  colormapImageFilter->SetColormap( RGBFilterType::Jet );
  colormapImageFilter->Update();
  
  using WriterType = itk::ImageFileWriter< RGBImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( colormapImageFilter->GetOutput() );
  writer->SetFileName("output.png");
  writer->Update();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create a black image with three white squares
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(200);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());

  // Make a square
  while(!imageIterator.IsAtEnd())
    {
    if(((imageIterator.GetIndex()[0] > 5 && imageIterator.GetIndex()[0] < 20) &&
      (imageIterator.GetIndex()[1] > 5 && imageIterator.GetIndex()[1] < 20)) ||
      ((imageIterator.GetIndex()[0] > 50 && imageIterator.GetIndex()[0] < 60) &&
      (imageIterator.GetIndex()[1] > 50 && imageIterator.GetIndex()[1] < 60)) ||
      ((imageIterator.GetIndex()[0] > 100 && imageIterator.GetIndex()[0] < 130) &&
      (imageIterator.GetIndex()[1] > 100 && imageIterator.GetIndex()[1] < 130))  )
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
  writer->SetFileName("input.png");
  writer->SetInput(image);
  writer->Update();
  
}
