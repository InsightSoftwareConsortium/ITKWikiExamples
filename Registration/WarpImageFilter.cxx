#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkVector.h"
#if ITK_VERSION_MAJOR < 4
#include "itkDeformationFieldSource.h"
#else
#include "itkLandmarkDisplacementFieldSource.h"
#endif

#include "itkWarpImageFilter.h"

constexpr unsigned int Dimension = 2;
using PixelType = unsigned char;
using ImageType = itk::Image< PixelType, Dimension >;

static void CreateFixedImage(ImageType::Pointer image);
static void CreateMovingImage(ImageType::Pointer image);
  
int main(int, char *[])
{
  using VectorComponentType = float;

  using VectorType = itk::Vector< VectorComponentType, Dimension >;
  using DeformationFieldType = itk::Image< VectorType,  Dimension >;

  ImageType::Pointer fixedImage =
    ImageType::New();
  CreateFixedImage(fixedImage);
  
  ImageType::Pointer movingImage =
    ImageType::New();
  CreateMovingImage(movingImage);
  
#if ITK_VERSION_MAJOR < 4
  using DeformationFieldSourceType = itk::DeformationFieldSource<DeformationFieldType>;
#else
  using DeformationFieldSourceType = itk::LandmarkDisplacementFieldSource<DeformationFieldType>;
#endif
  DeformationFieldSourceType::Pointer deformationFieldSource =
    DeformationFieldSourceType::New();
  deformationFieldSource->SetOutputSpacing( fixedImage->GetSpacing() );
  deformationFieldSource->SetOutputOrigin(  fixedImage->GetOrigin() );
  deformationFieldSource->SetOutputRegion(  fixedImage->GetLargestPossibleRegion() );
  deformationFieldSource->SetOutputDirection( fixedImage->GetDirection() );

  //  Create source and target landmarks.
  using LandmarkContainerType = DeformationFieldSourceType::LandmarkContainer;
  using LandmarkPointType = DeformationFieldSourceType::LandmarkPointType;

  LandmarkContainerType::Pointer sourceLandmarks =
    LandmarkContainerType::New();
  LandmarkContainerType::Pointer targetLandmarks =
    LandmarkContainerType::New();

  LandmarkPointType sourcePoint;
  LandmarkPointType targetPoint;

  sourcePoint[0] = 40;
  sourcePoint[1] = 40;
  targetPoint[0] = 20;
  targetPoint[1] = 20;
  sourceLandmarks->InsertElement( 0, sourcePoint );
  targetLandmarks->InsertElement( 0, targetPoint );

  sourcePoint[0] = 40;
  sourcePoint[1] = 60;
  targetPoint[0] = 20;
  targetPoint[1] = 80;
  sourceLandmarks->InsertElement( 1, sourcePoint );
  targetLandmarks->InsertElement( 1, targetPoint );

  sourcePoint[0] = 60;
  sourcePoint[1] = 40;
  targetPoint[0] = 80;
  targetPoint[1] = 20;
  sourceLandmarks->InsertElement( 2, sourcePoint );
  targetLandmarks->InsertElement( 2, targetPoint );

  sourcePoint[0] = 60;
  sourcePoint[1] = 60;
  targetPoint[0] = 80;
  targetPoint[1] = 80;
  sourceLandmarks->InsertElement( 3, sourcePoint );
  targetLandmarks->InsertElement( 3, targetPoint );

  deformationFieldSource->SetSourceLandmarks( sourceLandmarks.GetPointer() );
  deformationFieldSource->SetTargetLandmarks( targetLandmarks.GetPointer() );
  deformationFieldSource->UpdateLargestPossibleRegion();

  // Write the deformation field
  {
  using WriterType = itk::ImageFileWriter<  DeformationFieldType  >;
  WriterType::Pointer writer =
    WriterType::New();
  writer->SetInput (  deformationFieldSource->GetOutput() );
  writer->SetFileName( "deformation.mhd" );
  writer->Update();
  }
  

  using WarpImageFilterType = itk::WarpImageFilter< ImageType,
    ImageType,
    DeformationFieldType  >;

  WarpImageFilterType::Pointer warpImageFilter =
    WarpImageFilterType::New();

  using InterpolatorType = itk::LinearInterpolateImageFunction<ImageType, double >;

  InterpolatorType::Pointer interpolator =
    InterpolatorType::New();
  warpImageFilter->SetInterpolator( interpolator );
  warpImageFilter->SetOutputSpacing( deformationFieldSource->GetOutput()->GetSpacing() );
  warpImageFilter->SetOutputOrigin(  deformationFieldSource->GetOutput()->GetOrigin() );
#if ITK_VERSION_MAJOR < 4
  warpImageFilter->SetDeformationField( deformationFieldSource->GetOutput() );
#else
  warpImageFilter->SetDisplacementField( deformationFieldSource->GetOutput() );
#endif
  warpImageFilter->SetInput( movingImage );
  warpImageFilter->Update();

  // Write the output
  using WriterType = itk::ImageFileWriter<  ImageType  >;
  WriterType::Pointer writer =
    WriterType::New();
  writer->SetInput (  warpImageFilter->GetOutput() );
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
    if(imageIterator.GetIndex()[0] > 40 && imageIterator.GetIndex()[0] < 60 &&
       imageIterator.GetIndex()[1] > 40 && imageIterator.GetIndex()[1] < 60)
      {
      imageIterator.Set(255);
      }
    ++imageIterator;
    }

  // Write the deformation field
  using WriterType = itk::ImageFileWriter<  ImageType  >;
  WriterType::Pointer writer =
    WriterType::New();
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
    if(imageIterator.GetIndex()[0] > 20 && imageIterator.GetIndex()[0] < 80 &&
       imageIterator.GetIndex()[1] > 20 && imageIterator.GetIndex()[1] < 80)
      {
      imageIterator.Set(100);
      }
    ++imageIterator;
    }

  // Write the deformation field
  using WriterType = itk::ImageFileWriter<  ImageType  >;
  WriterType::Pointer writer =
    WriterType::New();
  writer->SetInput (  image );
  writer->SetFileName( "moving.png" );
  writer->Update();
}
