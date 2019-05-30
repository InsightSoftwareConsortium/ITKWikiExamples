#include "itkSpatialObjectToImageFilter.h"
#include "itkLineSpatialObject.h"
#include "itkLineSpatialObjectPoint.h"
#include "itkImageFileWriter.h"

int main( int itkNotUsed( argc ), char * itkNotUsed( argv )[] )
{
  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 2;

  using ImageType = itk::Image< PixelType, Dimension >;

  using LineType = itk::LineSpatialObject< Dimension >;

  using SpatialObjectToImageFilterType = itk::SpatialObjectToImageFilter<
    LineType, ImageType >;


  // Create a list of points
  std::vector<LineType::LinePointType> points;
  for(unsigned int i = 0; i < 20; i++)
    {
    LineType::LinePointType point;
    point.SetPositionInObjectSpace(10,i);

    LineType::LinePointType::CovariantVectorType normal;
    normal[0] = 0;
    normal[1] = 1;
    point.SetNormalInObjectSpace(normal,0);
    points.push_back(point);
    }

  // Create a line from the list of points
  LineType::Pointer line = LineType::New();
  line->SetPoints(points);

  SpatialObjectToImageFilterType::Pointer imageFilter =
    SpatialObjectToImageFilterType::New();
  itk::Size<2> size;
  size.Fill(50);
  imageFilter->SetInsideValue(255); // white
  imageFilter->SetSize(size);
  imageFilter->SetInput(line);
  imageFilter->Update();
  
  using WriterType = itk::ImageFileWriter< ImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("line.png");
  writer->SetInput( imageFilter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
