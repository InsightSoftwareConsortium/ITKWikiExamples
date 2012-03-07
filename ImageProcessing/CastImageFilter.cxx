#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkCastImageFilter.h"

#include <itkImageToVTKImageFilter.h>

#include "vtkImageViewer.h"]
#include "vtkImageMapper3D.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;
  typedef itk::Image<float, 2>  FloatImageType;

  typedef itk::ImageFileReader<FloatImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::CastImageFilter< FloatImageType, UnsignedCharImageType > CastFilterType;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(reader->GetOutput());

  typedef itk::ImageToVTKImageFilter<UnsignedCharImageType> ConnectorType;
  ConnectorType::Pointer connector = ConnectorType::New();
  connector->SetInput(castFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
#if VTK_MAJOR_VERSION <= 5
  actor->SetInput(connector->GetOutput());
#else
  actor->GetMapper()->SetInputData(connector->GetOutput());
#endif
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
