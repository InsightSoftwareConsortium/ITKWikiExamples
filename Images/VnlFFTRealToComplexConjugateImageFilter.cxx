#include "itkImage.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkVnlFFTRealToComplexConjugateImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "itkImageFileReader.h"
#include "itkCastImageFilter.h"

#include <itksys/SystemTools.hxx>
#include "vnl/vnl_sample.h"
#include <math.h>

#include <itkImageToVTKImageFilter.h>

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"

int main(int argc, char*argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }
    
  typedef itk::Image<unsigned char, 2> UnsignedCharImageType;
  typedef itk::Image<float, 2> FloatImageType;
  typedef itk::ImageFileReader<FloatImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::VnlFFTRealToComplexConjugateImageFilter<FloatImageType::PixelType, 2> FFTType;
  FFTType::Pointer fftFilter = FFTType::New();
  fftFilter->SetInput(reader->GetOutput());
  fftFilter->Update();

  typedef itk::ComplexToRealImageFilter<FFTType::OutputImageType, UnsignedCharImageType> RealFilterType;
  RealFilterType::Pointer realFilter = RealFilterType::New();
  realFilter->SetInput(fftFilter->GetOutput());
  realFilter->Update();

  typedef itk::RescaleIntensityImageFilter<UnsignedCharImageType, UnsignedCharImageType> RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(realFilter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();
  
  // Visualize original image
  typedef itk::CastImageFilter< FloatImageType, UnsignedCharImageType > CastFilterType;
  CastFilterType::Pointer inputCastFilter = CastFilterType::New();
  inputCastFilter->SetInput(reader->GetOutput());
  
  typedef itk::ImageToVTKImageFilter<UnsignedCharImageType> ConnectorType;
  ConnectorType::Pointer originalConnector = ConnectorType::New();
  originalConnector->SetInput(inputCastFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> originalActor =
    vtkSmartPointer<vtkImageActor>::New();
  originalActor->SetInput(originalConnector->GetOutput());

  // Visualize FFT
  typedef itk::ImageToVTKImageFilter<UnsignedCharImageType> ConnectorType;
  ConnectorType::Pointer fftConnector = ConnectorType::New();
  fftConnector->SetInput(rescaleFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> fftActor =
    vtkSmartPointer<vtkImageActor>::New();
  fftActor->SetInput(fftConnector->GetOutput());

  // Define viewport ranges
  // (xmin, ymin, xmax, ymax)
  double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
  double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

  // Setup both renderers
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(600,300);

  vtkSmartPointer<vtkRenderer> leftRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(leftRenderer);
  leftRenderer->SetViewport(leftViewport);

  vtkSmartPointer<vtkRenderer> rightRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(rightRenderer);
  rightRenderer->SetViewport(rightViewport);

  leftRenderer->AddActor(originalActor);
  rightRenderer->AddActor(fftActor);

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();

  renderWindowInteractor->SetInteractorStyle(style);

  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->Initialize();

  renderWindowInteractor->Start();
  
  return EXIT_SUCCESS;
}
