#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkLabelShapeKeepNObjectsImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

int main( int argc, char *argv[])
{
  if( argc < 2 )
    {
    std::cout << "Usage:" << std::endl;
    std::cout << argv[0] << " InputFileName" << std::endl;
    }
  constexpr unsigned int Dimension = 2;
  using PixelType = unsigned char;
  using ImageType = itk::Image< PixelType, Dimension >;

  using ReaderType = itk::ImageFileReader< ImageType >;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  using OutputImageType = itk::Image< unsigned short, Dimension >;

  using ConnectedComponentImageFilterType = itk::ConnectedComponentImageFilter <ImageType, OutputImageType >;

  ConnectedComponentImageFilterType::Pointer connected = ConnectedComponentImageFilterType::New ();
  connected->SetInput(reader->GetOutput());
  connected->Update();

  std::cout << "Number of objects: " << connected->GetObjectCount() << std::endl;

  using LabelShapeKeepNObjectsImageFilterType = itk::LabelShapeKeepNObjectsImageFilter< OutputImageType >;
  LabelShapeKeepNObjectsImageFilterType::Pointer labelShapeKeepNObjectsImageFilter = LabelShapeKeepNObjectsImageFilterType::New();
  labelShapeKeepNObjectsImageFilter->SetInput( connected->GetOutput() );
  labelShapeKeepNObjectsImageFilter->SetBackgroundValue( 0 );
  labelShapeKeepNObjectsImageFilter->SetNumberOfObjects( 1 );
  labelShapeKeepNObjectsImageFilter->SetAttribute( LabelShapeKeepNObjectsImageFilterType::LabelObjectType::NUMBER_OF_PIXELS);

  using RescaleFilterType = itk::RescaleIntensityImageFilter< OutputImageType, ImageType >;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(itk::NumericTraits<PixelType>::max());
  rescaleFilter->SetInput(labelShapeKeepNObjectsImageFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "Largest object of " << connected->GetObjectCount() << " objects";
  viewer.AddImage(
    rescaleFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
