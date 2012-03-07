#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkLabelShapeKeepNObjectsImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int main( int argc, char *argv[])
{
  if( argc < 3 )
    {
    std::cout << "Usage:" << std::endl;
    std::cout << argv[0] << " InputFileName OutputFileName" << std::endl;
    }
  const unsigned int Dimension = 3;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, Dimension >  ImageType;

  typedef itk::ImageFileReader< ImageType >   ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::Image< unsigned short, Dimension > OutputImageType;

  typedef itk::ConnectedComponentImageFilter <ImageType, OutputImageType >
    ConnectedComponentImageFilterType;

  ConnectedComponentImageFilterType::Pointer connected = ConnectedComponentImageFilterType::New ();
  connected->SetInput(reader->GetOutput());
  connected->Update();

  std::cout << "Number of objects: " << connected->GetObjectCount() << std::endl;

  typedef itk::LabelShapeKeepNObjectsImageFilter< OutputImageType > LabelShapeKeepNObjectsImageFilterType;
  LabelShapeKeepNObjectsImageFilterType::Pointer labelShapeKeepNObjectsImageFilter = LabelShapeKeepNObjectsImageFilterType::New();
  labelShapeKeepNObjectsImageFilter->SetInput( connected->GetOutput() );
  labelShapeKeepNObjectsImageFilter->SetBackgroundValue( 0 );
  labelShapeKeepNObjectsImageFilter->SetNumberOfObjects( 1 );
  labelShapeKeepNObjectsImageFilter->SetAttribute( LabelShapeKeepNObjectsImageFilterType::LabelObjectType::NUMBER_OF_PIXELS);
  labelShapeKeepNObjectsImageFilter->Update();

  typedef itk::RescaleIntensityImageFilter< OutputImageType, ImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(itk::NumericTraits<PixelType>::max());
  rescaleFilter->SetInput(labelShapeKeepNObjectsImageFilter->GetOutput());
  rescaleFilter->Update();

  typedef itk::ImageFileWriter< ImageType >   WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( rescaleFilter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
