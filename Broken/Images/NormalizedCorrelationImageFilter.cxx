#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkNormalizedCorrelationImageFilter.h>
#include <itkRegionOfInterestImageFilter.h>
#include <itkImageKernelOperator.h>
#include <itkCastImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

#include <itkImageToVTKImageFilter.h>

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string filename = argv[1];

  typedef itk::Image<unsigned char, 2> UnsignedCharImageType;
  typedef itk::Image<float, 2> FloatImageType;
  typedef itk::ImageFileReader<FloatImageType> ReaderType;
  typedef itk::ImageToVTKImageFilter<UnsignedCharImageType> ConnectorType;

  // Read the image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename.c_str());

  // Setup the original image actor
  typedef itk::CastImageFilter< FloatImageType,
                                UnsignedCharImageType > CastFilterType;
  CastFilterType::Pointer originalCastFilter = CastFilterType::New();
  originalCastFilter->SetInput(reader->GetOutput());
  
  ConnectorType::Pointer originalConnector = ConnectorType::New();
  originalConnector->SetInput(originalCastFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> originalActor =
    vtkSmartPointer<vtkImageActor>::New();
  originalActor->SetInput(originalConnector->GetOutput());

  // Extract a small region
  typedef itk::RegionOfInterestImageFilter< FloatImageType,
                                            FloatImageType > ExtractFilterType;

  ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();

  UnsignedCharImageType::IndexType start;
  start[0] = 50;
  start[1] = 50;

  UnsignedCharImageType::SizeType size;
  size[0] = 100;
  size[1] = 100;

  UnsignedCharImageType::RegionType desiredRegion;
  desiredRegion.SetSize(size);
  desiredRegion.SetIndex(start);

  extractFilter->SetRegionOfInterest(desiredRegion);
  extractFilter->SetInput(reader->GetOutput());

  // Display extracted region (kernel)
  CastFilterType::Pointer kernelCastFilter = CastFilterType::New();
  kernelCastFilter->SetInput(extractFilter->GetOutput());
  
  ConnectorType::Pointer extractedConnector = ConnectorType::New();
  extractedConnector->SetInput(kernelCastFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> extractedActor =
    vtkSmartPointer<vtkImageActor>::New();
  extractedActor->SetInput(extractedConnector->GetOutput());

  // Perform normalized correlation
  // <input type, mask type (not used), output type>
  typedef itk::NormalizedCorrelationImageFilter<FloatImageType, FloatImageType, FloatImageType> CorrelationFilterType;

  itk::ImageKernelOperator<float> kernelOperator;
  kernelOperator.SetImageKernel(extractFilter->GetOutput());
  
  CorrelationFilterType::Pointer correlationFilter = CorrelationFilterType::New();
  correlationFilter->SetInput(reader->GetOutput());
  correlationFilter->SetTemplate(kernelOperator);

  // Display the correlation image
  typedef itk::RescaleIntensityImageFilter< FloatImageType, UnsignedCharImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(correlationFilter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  ConnectorType::Pointer correlationConnector = ConnectorType::New();
  correlationConnector->SetInput(rescaleFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> correlationActor =
    vtkSmartPointer<vtkImageActor>::New();
  correlationActor->SetInput(correlationConnector->GetOutput());

  // Visualize
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(900, 300);

  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  double leftViewport[4] = {0.0, 0.0, 0.33, 1.0};
  double centerViewport[4] = {0.33, 0.0, 0.66, 1.0};
  double rightViewport[4] = {0.66, 0.0, 1.0, 1.0};

  vtkSmartPointer<vtkRenderer> leftRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(leftRenderer);
  leftRenderer->SetViewport(leftViewport);
  leftRenderer->SetBackground(.6, .5, .4);

  vtkSmartPointer<vtkRenderer> centerRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(centerRenderer);
  centerRenderer->SetViewport(centerViewport);
  centerRenderer->SetBackground(.5, .5, .6);

  vtkSmartPointer<vtkRenderer> rightRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(rightRenderer);
  rightRenderer->SetViewport(rightViewport);
  rightRenderer->SetBackground(.4, .5, .6);

  leftRenderer->AddActor(originalActor);
  centerRenderer->AddActor(extractedActor);
  rightRenderer->AddActor(correlationActor);

  leftRenderer->ResetCamera();
  rightRenderer->ResetCamera();

  renderWindow->Render();

  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  interactor->SetInteractorStyle(style);

  interactor->Start();

  return EXIT_SUCCESS;
}
