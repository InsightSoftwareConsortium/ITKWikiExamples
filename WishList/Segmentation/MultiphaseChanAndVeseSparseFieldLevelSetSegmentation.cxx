#include "itkScalarChanAndVeseSparseLevelSetImageFilter.h"
#include "itkScalarChanAndVeseLevelSetFunction.h"
#include "itkScalarChanAndVeseLevelSetFunctionData.h"
#include "itkConstrainedRegionBasedLevelSetFunctionSharedData.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkAtanRegularizedHeavisideStepFunction.h"

int main(int argc, char**argv)
{
  if( argc < 10 )
  {
    std::cerr << "Missing arguments" << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0];
    std::cerr << " inputLevelSetImage1 inputLevelSetImage2 inputLevelSetImage3";
    std::cerr << " inputFeatureImage outputLevelSetImage";
    std::cerr << " CurvatureWeight AreaWeight LaplacianWeight";
    std::cerr << " VolumeWeight Volume OverlapWeight" << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int nb_iteration = 50;
  double rms = 0.;
  double epsilon = 1.5;
  double curvature_weight = atof( argv[6] );
  double area_weight = atof( argv[7] );
  double volume_weight = atof( argv[8] );
  double volume = atof( argv[9] );
  double overlap_weight = atof( argv[10] );
  double l1 = 1.;
  double l2 = 1.;

  const unsigned int Dimension = 2;
  typedef float ScalarPixelType;

  typedef itk::Image< ScalarPixelType, Dimension > LevelSetImageType;
  typedef itk::Image< unsigned char, Dimension > FeatureImageType;
  typedef itk::Image< unsigned char, Dimension > OutputImageType;

  typedef itk::ScalarChanAndVeseLevelSetFunctionData< LevelSetImageType,
    FeatureImageType > DataHelperType;

  typedef itk::ConstrainedRegionBasedLevelSetFunctionSharedData<
    LevelSetImageType, FeatureImageType, DataHelperType > SharedDataHelperType;

  typedef itk::ScalarChanAndVeseLevelSetFunction< LevelSetImageType,
    FeatureImageType, SharedDataHelperType > LevelSetFunctionType;

  typedef itk::ScalarChanAndVeseSparseLevelSetImageFilter< LevelSetImageType,
    FeatureImageType, OutputImageType, LevelSetFunctionType,
    SharedDataHelperType > MultiLevelSetType;

  typedef itk::ImageFileReader< LevelSetImageType > LevelSetReaderType;
  typedef itk::ImageFileReader< FeatureImageType > FeatureReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  typedef itk::AtanRegularizedHeavisideStepFunction< ScalarPixelType,
    ScalarPixelType >  DomainFunctionType;

  DomainFunctionType::Pointer domainFunction = DomainFunctionType::New();
  domainFunction->SetEpsilon( epsilon );

  LevelSetReaderType::Pointer contourReader1 = LevelSetReaderType::New();
  contourReader1->SetFileName( argv[1] );
  contourReader1->Update();

  LevelSetReaderType::Pointer contourReader2 = LevelSetReaderType::New();
  contourReader2->SetFileName( argv[2] );
  contourReader2->Update();

  LevelSetReaderType::Pointer contourReader3 = LevelSetReaderType::New();
  contourReader3->SetFileName( argv[3] );
  contourReader3->Update();

  FeatureReaderType::Pointer featureReader = FeatureReaderType::New();
  featureReader->SetFileName( argv[4] );
  featureReader->Update();

  FeatureImageType::Pointer featureImage = featureReader->GetOutput();
  LevelSetImageType::Pointer contourImage1 = contourReader1->GetOutput();
  LevelSetImageType::Pointer contourImage2 = contourReader2->GetOutput();
  LevelSetImageType::Pointer contourImage3 = contourReader3->GetOutput();

  MultiLevelSetType::Pointer levelSetFilter = MultiLevelSetType::New();
  levelSetFilter->SetFunctionCount( 3 );
  levelSetFilter->SetFeatureImage( featureImage );
  levelSetFilter->SetLevelSet( 0, contourImage1 );
  levelSetFilter->SetLevelSet( 1, contourImage2 );
  levelSetFilter->SetLevelSet( 2, contourImage3 );
  levelSetFilter->SetNumberOfIterations( nb_iteration );
  levelSetFilter->SetMaximumRMSError( rms );
  levelSetFilter->SetUseImageSpacing( 1 );
  levelSetFilter->SetInPlace( false );

  for ( unsigned int i = 0; i < 3; i++ )
  {
    levelSetFilter->GetDifferenceFunction(i)->SetDomainFunction( domainFunction );
    levelSetFilter->GetDifferenceFunction(i)->SetCurvatureWeight( curvature_weight );
    levelSetFilter->GetDifferenceFunction(i)->SetAreaWeight( area_weight );
    levelSetFilter->GetDifferenceFunction(i)->SetOverlapPenaltyWeight( overlap_weight );
    levelSetFilter->GetDifferenceFunction(i)->SetVolumeMatchingWeight( volume_weight );
    levelSetFilter->GetDifferenceFunction(i)->SetVolume( volume );
    levelSetFilter->GetDifferenceFunction(i)->SetLambda1( l1 );
    levelSetFilter->GetDifferenceFunction(i)->SetLambda2( l2 );
  }

  levelSetFilter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( levelSetFilter->GetOutput() );
  writer->SetFileName( argv[5] );

  try
  {
    writer->Update();
  }
  catch( itk::ExceptionObject & excep )
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return -1;
  }

  return EXIT_SUCCESS;
}
