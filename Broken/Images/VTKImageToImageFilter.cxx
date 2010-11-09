#include <itkImage.h>
#include <itkImageFileReader.h>

#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkPNGReader.h"

int main(int argc, char*argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  vtkSmartPointer<vtkPNGReader> reader =
    vtkSmartPointer<vtkPNGReader>::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  
  typedef itk::Image<unsigned char, 2> ImageType;

  typedef itk::ImageToVTKImageFilter<ImageType> ImageToVTKImageType;
  typedef itk::VTKImageToImageFilter<ImageType> VTKImageToImageType;

  VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
  vtkImageToImageFilter->SetInput(reader->GetOutput());
  
  ImageToVTKImageType::Pointer imageToVTKImageFilter = ImageToVTKImageType::New();
  imageToVTKImageFilter->SetInput(vtkImageToImageFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
  actor->SetInput(imageToVTKImageFilter->GetOutput());

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  renderer->ResetCamera();

  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

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
