#include <itkImageFileReader.h>
#include <itkImageRegionIterator.h>
#include <itkScalarImageToListAdaptor.h>
#include <itkKdTree.h>
#include <itkKdTreeGenerator.h>
#include <itkMeanShiftModeCacheMethod.h>
#include <itkHypersphereKernelMeanShiftModeSeeker.h>
#include <itkSampleMeanShiftBlurringFilter.h>
#include <itkSampleMeanShiftClusteringFilter.h>

#include <itkImageFileWriter.h>


int main(int argc, char* argv[] ) 
{
  if (argc < 2)
    {
      std::cout << "ERROR: data file name argument missing." 
                << std::endl;
      return EXIT_FAILURE;
    }

  std::string inputFilename = argv[1];
  
  using PixelType = unsigned char;
  using ImageType = itk::Image< PixelType, 2 >;
  using ImageReaderType = itk::ImageFileReader< ImageType >;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();

  imageReader->SetFileName(inputFilename.c_str());
  imageReader->Update();
  ImageType::Pointer image = imageReader->GetOutput();
  
  using ListSampleType = itk::Statistics::ScalarImageToListAdaptor< ImageType >;
  
  ListSampleType::Pointer listSample = 
    ListSampleType::New();
  listSample->SetImage( image );

  using TreeGeneratorType = itk::Statistics::KdTreeGenerator< ListSampleType >;
  TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();
  treeGenerator->SetSample( listSample );
  treeGenerator->SetBucketSize( 200 );
  treeGenerator->Update();

  using TreeType = TreeGeneratorType::KdTreeType;
  TreeType::Pointer tree = treeGenerator->GetOutput();

  using ModeSeekerType = itk::Statistics::HypersphereKernelMeanShiftModeSeeker< 
    TreeType >;
  ModeSeekerType::Pointer modeSeeker = ModeSeekerType::New();
  modeSeeker->SetInputSample( tree );
//  modeSeeker->SetInputSample( listSample );
  modeSeeker->SetSearchRadius( 4.0 );

  using CacheMethodType = itk::Statistics::MeanShiftModeCacheMethod< TreeType::MeasurementVectorType >;
  CacheMethodType::Pointer cacheMethod = CacheMethodType::New();
  cacheMethod->SetMaximumEntries(255);
  cacheMethod->SetMaximumConsecutiveFailures(100);
  cacheMethod->SetHitRatioThreshold( 0.5 );
  modeSeeker->SetCacheMethod( cacheMethod.GetPointer() );

  using FilterType = itk::Statistics::SampleMeanShiftBlurringFilter< TreeType >;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInputSample( tree );
  filter->SetMeanShiftModeSeeker( modeSeeker );
  filter->Update();

  std::cout << "Cache statistics: " << std::endl;
  cacheMethod->Print(std::cout);

  using OutputImageType = ImageType;
  OutputImageType::Pointer outputImage = OutputImageType::New();
  outputImage->SetRegions( image->GetLargestPossibleRegion() );
  outputImage->Allocate();

  using ImageIteratorType = itk::ImageRegionIterator< OutputImageType >;
  ImageIteratorType io_iter( outputImage,
                             outputImage->GetLargestPossibleRegion() );
  io_iter.GoToBegin();

  FilterType::OutputType::Pointer output = filter->GetOutput();
  FilterType::OutputType::Iterator fo_iter = output->Begin();
  FilterType::OutputType::Iterator fo_end = output->End();

  while ( fo_iter != fo_end )
    {
    io_iter.Set( (PixelType) fo_iter.GetMeasurementVector()[0]);
    ++fo_iter;
    ++io_iter;
    }

  ListSampleType::Pointer listSample2 = ListSampleType::New();
  listSample2->SetImage( outputImage );

  TreeGeneratorType::Pointer treeGenerator2 = TreeGeneratorType::New();
  treeGenerator2->SetSample( listSample2 );
  treeGenerator2->SetBucketSize( 200 );
  treeGenerator2->Update();

  using ClusteringMethodType = itk::Statistics::SampleMeanShiftClusteringFilter< TreeType >;

  ClusteringMethodType::Pointer clusteringMethod =
    ClusteringMethodType::New();
  clusteringMethod->SetInputSample( treeGenerator2->GetOutput() );
  clusteringMethod->SetThreshold( 0.5 );
  clusteringMethod->SetMinimumClusterSize( 16 );
  clusteringMethod->DebugOn();
  clusteringMethod->Update();

  // save clustered image
  OutputImageType::Pointer clusterMap = OutputImageType::New();
  clusterMap->SetRegions( image->GetLargestPossibleRegion() );
  clusterMap->Allocate();
  
  ImageIteratorType m_iter( clusterMap, 
                            clusterMap->GetLargestPossibleRegion() );
  m_iter.GoToBegin();
  
  ClusteringMethodType::ClusterLabelsType clusterLabels = 
    clusteringMethod->GetOutput();
  
  ClusteringMethodType::ClusterLabelsType::iterator co_iter = 
    clusterLabels.begin();
  
  while ( co_iter != clusterLabels.end() )
    {
    m_iter.Set( (PixelType) *co_iter );
    ++co_iter;
    ++m_iter;
    }
  
  using ImageWriterType = itk::ImageFileWriter< OutputImageType >;
  ImageWriterType::Pointer map_writer = ImageWriterType::New();
  map_writer->SetFileName("clustered_sf4.png");
  map_writer->SetInput( clusterMap );
  map_writer->Update();


  return EXIT_SUCCESS;
}
