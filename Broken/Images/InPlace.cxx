#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkBinaryThresholdImageFilter.h"

#include <itkImageToVTKImageFilter.h>

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"

using ImageType = itk::Image< unsigned char, 2 >;

static void ApplyThresholding(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }
    
  std::string inputFilename = argv[1];

  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  ImageType::Pointer image = reader->GetOutput();

  ApplyThresholding(image);
  
  // Visualize
  using ConnectorType = itk::ImageToVTKImageFilter<ImageType>;
  ConnectorType::Pointer connector = ConnectorType::New();
  connector->SetInput(image);

  vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
  actor->SetInput(connector->GetOutput());

  // There will be one render window
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();

  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(renderer);

  renderer->AddActor(actor);
  renderer->ResetCamera();

  renderWindow->Render();

  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();

  interactor->SetInteractorStyle(style);

  interactor->Start();
  
  return EXIT_SUCCESS;
}

void ApplyThresholding(ImageType::Pointer image)
{

  using BinaryThresholdImageFilterType = itk::BinaryThresholdImageFilter <ImageType, ImageType>;

  BinaryThresholdImageFilterType::Pointer thresholdFilter
          = BinaryThresholdImageFilterType::New();
  thresholdFilter->SetInput(image);
  thresholdFilter->SetLowerThreshold(10);
  thresholdFilter->SetUpperThreshold(50);
  thresholdFilter->SetInsideValue(255);
  thresholdFilter->SetOutsideValue(0);
  thresholdFilter->InPlaceOn();
  thresholdFilter->Update();
}
