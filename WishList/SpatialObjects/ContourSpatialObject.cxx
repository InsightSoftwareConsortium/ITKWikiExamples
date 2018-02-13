#include "itkSpatialObjectToImageFilter.h"
#include "itkContourSpatialObject.h"
#include "itkContourSpatialObjectPoint.h"
#include "itkImageFileWriter.h"

#include "QuickView.h"

int main( int argc, char *argv[] )
{
  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 2;

  using ImageType = itk::Image< PixelType, Dimension >;

  using ContourType = itk::ContourSpatialObject< Dimension >;

  using SpatialObjectToImageFilterType = itk::SpatialObjectToImageFilter<
    ContourType, ImageType >;


  // Create a list of points
  ContourType::ControlPointListType points;

  // Add some points
  ContourType::ControlPointType point;
  point.SetPosition(0,0);
  points.push_back(point);
  point.SetPosition(0,30);
  points.push_back(point);
  point.SetPosition(30,30);
  points.push_back(point);
  point.SetPosition(0,0);
  points.push_back(point);
  
  // Create a contour from the list of points
  ContourType::Pointer contour = ContourType::New();
  contour->SetControlPoints(points);

  SpatialObjectToImageFilterType::Pointer imageFilter =
    SpatialObjectToImageFilterType::New();
  itk::Size<2> size;
  size.Fill(50);
  imageFilter->SetInsideValue(255); // white
  imageFilter->SetSize(size);
  imageFilter->SetInput(contour);
  imageFilter->Update();

  QuickView viewer;
  viewer.AddImage(imageFilter->GetOutput());
  viewer.Visualize();
  /*
  using WriterType = itk::ImageFileWriter< ImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("contour.png");
  writer->SetInput( imageFilter->GetOutput() );
  writer->Update();
  */
  return EXIT_SUCCESS;
}
