#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkVector.h"
#if ITK_VERSION_MAJOR < 4
#include "itkDeformationFieldTransform.h"
#include "itkDeformationFieldSource.h"
#else
#include "itkVectorLinearInterpolateImageFunction.h"
#include "itkDisplacementFieldTransform.h"
#include "itkLandmarkDisplacementFieldSource.h"
#endif
#include "itkResampleImageFilter.h"

const     unsigned int   Dimension = 2;
typedef   unsigned char  PixelType;
typedef   itk::Image< PixelType, Dimension > ImageType;

static void CreateFixedImage(ImageType::Pointer image);
static void CreateMovingImage(ImageType::Pointer image);
  
int main(int argc, char * argv[])
{
#if ITK_VERSION_MAJOR < 4
  typedef   float           VectorComponentType;
#else
  typedef   double          VectorComponentType;
#endif
  typedef   itk::Vector< VectorComponentType, Dimension >    VectorType;
  typedef   itk::Image< VectorType,  Dimension >   DeformationFieldType;

  ImageType::Pointer fixedImage = ImageType::New();
  CreateFixedImage(fixedImage);
  
  ImageType::Pointer movingImage = ImageType::New();
  CreateMovingImage(movingImage);
  
#if ITK_VERSION_MAJOR < 4
  typedef itk::DeformationFieldSource<DeformationFieldType>  DeformationFieldSourceType;
#else
  typedef itk::LandmarkDisplacementFieldSource<DeformationFieldType>  DeformationFieldSourceType;
#endif
  DeformationFieldSourceType::Pointer deformationFieldSource = DeformationFieldSourceType::New();
  deformationFieldSource->SetOutputSpacing( fixedImage->GetSpacing() );
  deformationFieldSource->SetOutputOrigin(  fixedImage->GetOrigin() );
  deformationFieldSource->SetOutputRegion(  fixedImage->GetLargestPossibleRegion() );
  deformationFieldSource->SetOutputDirection( fixedImage->GetDirection() );

  //  Create source and target landmarks.
  typedef DeformationFieldSourceType::LandmarkContainerPointer   LandmarkContainerPointer;
  typedef DeformationFieldSourceType::LandmarkContainer          LandmarkContainerType;
  typedef DeformationFieldSourceType::LandmarkPointType          LandmarkPointType;

  LandmarkContainerType::Pointer sourceLandmarks = LandmarkContainerType::New();
  LandmarkContainerType::Pointer targetLandmarks = LandmarkContainerType::New();

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
  typedef itk::ImageFileWriter<  DeformationFieldType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  deformationFieldSource->GetOutput() );
  writer->SetFileName( "deformation.mhd" );
  writer->Update();
  }
  
#if ITK_VERSION_MAJOR < 4
  typedef itk::DeformationFieldTransform<VectorComponentType, 2>  DeformationFieldTransformType;
#else
  typedef itk::DisplacementFieldTransform<VectorComponentType, 2>  DeformationFieldTransformType;
#endif
  DeformationFieldTransformType::Pointer deformationFieldTransform = DeformationFieldTransformType::New();

#if ITK_VERSION_MAJOR < 4
  deformationFieldTransform->SetDeformationField( deformationFieldSource->GetOutput() );
#else
  deformationFieldTransform->SetDisplacementField( deformationFieldSource->GetOutput() );
#endif  
  typedef itk::ResampleImageFilter<ImageType, ImageType, VectorComponentType >    ResampleFilterType;
  ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
  resampleFilter->SetInput( movingImage );
  resampleFilter->SetTransform( deformationFieldTransform );
  resampleFilter->SetSize( fixedImage->GetLargestPossibleRegion().GetSize() );
  resampleFilter->SetOutputOrigin(  fixedImage->GetOrigin() );
  resampleFilter->SetOutputSpacing( fixedImage->GetSpacing() );
  resampleFilter->SetOutputDirection( fixedImage->GetDirection() );
  resampleFilter->SetDefaultPixelValue( 200 );
  resampleFilter->GetOutput();

  // Write the output
  typedef itk::ImageFileWriter<  ImageType  > WriterType;
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
    if(imageIterator.GetIndex()[0] > 40 && imageIterator.GetIndex()[0] < 60 &&
      imageIterator.GetIndex()[1] > 40 && imageIterator.GetIndex()[1] < 60)
      {
      imageIterator.Set(255);
      }
    ++imageIterator;
    }

  // Write the deformation field
  typedef itk::ImageFileWriter<  ImageType  > WriterType;
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
    if(imageIterator.GetIndex()[0] > 20 && imageIterator.GetIndex()[0] < 80 &&
      imageIterator.GetIndex()[1] > 20 && imageIterator.GetIndex()[1] < 80)
      {
      imageIterator.Set(100);
      }
    ++imageIterator;
    }

  // Write the deformation field
  typedef itk::ImageFileWriter<  ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput (  image );
  writer->SetFileName( "moving.png" );
  writer->Update();
}
