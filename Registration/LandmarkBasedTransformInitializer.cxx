#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkVector.h"
#include "itkResampleImageFilter.h"
#include "itkLandmarkBasedTransformInitializer.h"
#include "itkRigid2DTransform.h"

const     unsigned int   Dimension = 2;
using PixelType = unsigned char;
using ImageType = itk::Image< PixelType, Dimension >;

static void CreateFixedImage(ImageType::Pointer image);
static void CreateMovingImage(ImageType::Pointer image);
  
int main(int argc, char * argv[])
{
  using VectorComponentType = float;

  ImageType::Pointer fixedImage = ImageType::New();
  CreateFixedImage(fixedImage);
  
  ImageType::Pointer movingImage = ImageType::New();
  CreateMovingImage(movingImage);

  using Rigid2DTransformType = itk::Rigid2DTransform< double >;
  using LandmarkBasedTransformInitializerType = itk::LandmarkBasedTransformInitializer< Rigid2DTransformType, ImageType, ImageType >;
      
  LandmarkBasedTransformInitializerType::Pointer landmarkBasedTransformInitializer =
    LandmarkBasedTransformInitializerType::New();
  //  Create source and target landmarks.
  using LandmarkContainerType = LandmarkBasedTransformInitializerType::LandmarkPointContainer;
  using LandmarkPointType = LandmarkBasedTransformInitializerType::LandmarkPointType;

  LandmarkContainerType fixedLandmarks;
  LandmarkContainerType movingLandmarks;

  LandmarkPointType fixedPoint;
  LandmarkPointType movingPoint;

  fixedPoint[0] = 10;
  fixedPoint[1] = 10;
  movingPoint[0] = 50;
  movingPoint[1] = 50;
  fixedLandmarks.push_back( fixedPoint );
  movingLandmarks.push_back( movingPoint );

  fixedPoint[0] = 10;
  fixedPoint[1] = 20;
  movingPoint[0] = 50;
  movingPoint[1] = 60;
  fixedLandmarks.push_back( fixedPoint );
  movingLandmarks.push_back( movingPoint );

  fixedPoint[0] = 20;
  fixedPoint[1] = 10;
  movingPoint[0] = 60;
  movingPoint[1] = 50;
  fixedLandmarks.push_back( fixedPoint );
  movingLandmarks.push_back( movingPoint );

  fixedPoint[0] = 20;
  fixedPoint[1] = 20;
  movingPoint[0] = 60;
  movingPoint[1] = 60;
  fixedLandmarks.push_back( fixedPoint );
  movingLandmarks.push_back( movingPoint );

  landmarkBasedTransformInitializer->SetFixedLandmarks( fixedLandmarks );
  landmarkBasedTransformInitializer->SetMovingLandmarks( movingLandmarks );
  
  Rigid2DTransformType::Pointer transform = Rigid2DTransformType::New();
  
  transform->SetIdentity();
  landmarkBasedTransformInitializer->SetTransform(transform);
  landmarkBasedTransformInitializer->InitializeTransform();
  
  using ResampleFilterType = itk::ResampleImageFilter<ImageType, ImageType, double >;
  ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
  resampleFilter->SetInput( movingImage );
  resampleFilter->SetTransform( transform );
  resampleFilter->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
  resampleFilter->SetOutputOrigin(  fixedImage->GetOrigin() );
  resampleFilter->SetOutputSpacing( fixedImage->GetSpacing() );
  resampleFilter->SetOutputDirection( fixedImage->GetDirection() );
  resampleFilter->SetDefaultPixelValue( 200 );
  resampleFilter->GetOutput();

  // Write the output
  using WriterType = itk::ImageFileWriter<  ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  resampleFilter->GetOutput() );
  writer->SetFileName( "output.png" );
  writer->Update();
  
  return EXIT_SUCCESS;
}

void CreateFixedImage(ImageType::Pointer image)
{
  // Create a black image with a white square
  ImageType::IndexType start;
  start.Fill(0);
 
  ImageType::SizeType size;
  size.Fill(100);
 
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
 
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
 
  itk::ImageRegionIterator<ImageType> imageIterator(image,region);
 
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > 10 && imageIterator.GetIndex()[0] < 20 &&
      imageIterator.GetIndex()[1] > 10 && imageIterator.GetIndex()[1] < 20)
      {
      imageIterator.Set(255);
      }
    ++imageIterator;
    }

  // Write the deformation field
  using WriterType = itk::ImageFileWriter<  ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  image );
  writer->SetFileName( "fixed.png" );
  writer->Update();
}


void CreateMovingImage(ImageType::Pointer image)
{
  // Create a black image with a white square
  ImageType::IndexType start;
  start.Fill(0);
 
  ImageType::SizeType size;
  size.Fill(100);
 
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
 
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
 
  itk::ImageRegionIterator<ImageType> imageIterator(image,region);
 
  while(!imageIterator.IsAtEnd())
    {
    if(imageIterator.GetIndex()[0] > 50 && imageIterator.GetIndex()[0] < 60 &&
      imageIterator.GetIndex()[1] > 50 && imageIterator.GetIndex()[1] < 60)
      {
      imageIterator.Set(100);
      }
    ++imageIterator;
    }

  // Write the deformation field
  using WriterType = itk::ImageFileWriter<  ImageType  >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  image );
  writer->SetFileName( "moving.png" );
  writer->Update();
}
