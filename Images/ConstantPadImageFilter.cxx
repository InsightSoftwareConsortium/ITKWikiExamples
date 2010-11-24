#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkConstantPadImageFilter.h"
#include "itkImageRegionIterator.h"

#include "itkImageToVTKImageFilter.h"

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int, char *[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);

  typedef itk::ConstantPadImageFilter <ImageType, ImageType>
    ConstantPadImageFilterType;

  ImageType::SizeType lowerExtendRegion;
  lowerExtendRegion[0] = 20;
  lowerExtendRegion[1] = 30;

  ImageType::SizeType upperExtendRegion;
  upperExtendRegion[0] = 50;
  upperExtendRegion[1] = 40;

  ImageType::PixelType constantPixel = 100;

  ConstantPadImageFilterType::Pointer padFilter
    = ConstantPadImageFilterType::New();
  padFilter->SetInput(image);
  //padFilter->SetPadBound(outputRegion); // Calls SetPadLowerBound(region) and SetPadUpperBound(region)
  padFilter->SetPadLowerBound(lowerExtendRegion);
  padFilter->SetPadUpperBound(upperExtendRegion);
  padFilter->SetConstant(constantPixel);
  padFilter->Update();

  // Visualize first image
  typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
  ConnectorType::Pointer originalConnector = ConnectorType::New();
  originalConnector->SetInput(image);

  vtkSmartPointer<vtkImageActor> originalActor =
    vtkSmartPointer<vtkImageActor>::New();
  originalActor->SetInput(originalConnector->GetOutput());

  // Visualize padded image
  ConnectorType::Pointer paddedConnector = ConnectorType::New();
  paddedConnector->SetInput(padFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> paddedActor =
    vtkSmartPointer<vtkImageActor>::New();
  paddedActor->SetInput(paddedConnector->GetOutput());

  // There will be one render window
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(600, 300);

  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  // Define viewport ranges
  // (xmin, ymin, xmax, ymax)
  double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
  double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

  // Setup both renderers
  vtkSmartPointer<vtkRenderer> leftRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(leftRenderer);
  leftRenderer->SetViewport(leftViewport);
  leftRenderer->SetBackground(.6, .5, .4);

  vtkSmartPointer<vtkRenderer> rightRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(rightRenderer);
  rightRenderer->SetViewport(rightViewport);
  rightRenderer->SetBackground(.4, .5, .6);

  // Add the sphere to the left and the cube to the right
  leftRenderer->AddActor(originalActor);
  rightRenderer->AddActor(paddedActor);

  leftRenderer->ResetCamera();
  rightRenderer->ResetCamera();

  renderWindow->Render();

  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  interactor->SetInteractorStyle(style);

  interactor->Start();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  // Create an image
  ImageType::RegionType region;
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  // Make the whole image white
  itk::ImageRegionIterator<ImageType> iterator(image,image->GetLargestPossibleRegion());

  while(!iterator.IsAtEnd())
    {
    iterator.Set(255);
    ++iterator;
    }
}
