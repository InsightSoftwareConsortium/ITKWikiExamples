#include <itkImageFileReader.h>
#include "itkImageToVTKImageFilter.h"

#include <vtkSmartPointer.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkMatrix4x4.h>
#include <vtkAxesActor.h>

int main(int argc, char *argv[])
{
    typedef itk::Image<unsigned char, 3> VisualizingImageType;
    typedef itk::ImageFileReader< VisualizingImageType >  ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( argv[1] ); //http://www.cg.informatik.uni-siegen.de/People/Zukic/data/Downloads/DzZ_rot45.zip
    reader->Update();
    VisualizingImageType::Pointer image=reader->GetOutput();

    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
    ren1->SetBackground(0.5f,0.5f,1.0f);
  
    renWin->AddRenderer(ren1);
    renWin->SetSize(1280,1024);
    vtkSmartPointer<vtkRenderWindowInteractor> iren = 
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);
    renWin->Render(); // make sure we have an OpenGL context.
  
    typedef itk::ImageToVTKImageFilter<VisualizingImageType> itkVtkConverter;
    itkVtkConverter::Pointer conv=itkVtkConverter::New();
    conv->SetInput(image);
    
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = 
        vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    volumeMapper->SetInput(conv->GetOutput());
  
    vtkSmartPointer<vtkVolumeProperty> volumeProperty = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(0.0,0.0);
    compositeOpacity->AddPoint(80.0,1.0);
    compositeOpacity->AddPoint(80.1,0.0);
    compositeOpacity->AddPoint(255.0,0.0);
    volumeProperty->SetScalarOpacity(compositeOpacity);
  
    vtkSmartPointer<vtkColorTransferFunction> color = 
    vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0.0  ,0.0,0.0,1.0);
    color->AddRGBPoint(40.0  ,1.0,0.0,0.0);
    color->AddRGBPoint(255.0,1.0,1.0,1.0);
    volumeProperty->SetColor(color);

    vtkSmartPointer<vtkVolume> volume = 
    vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    //Here we take care of position and orientation
    //so that volume is in DICOM patient physical space
    VisualizingImageType::DirectionType d=image->GetDirection();
    vtkMatrix4x4 *mat=vtkMatrix4x4::New(); //start with identity matrix
    for (int i=0; i<3; i++)
        for (int k=0; k<3; k++)
            mat->SetElement(i,k, d(i,k));

    //counteract the built-in translation by origin
    VisualizingImageType::PointType origin=image->GetOrigin();
    volume->SetPosition(-origin[0], -origin[1], -origin[2]);

    //add translation to the user matrix
    for (int i=0; i<3; i++)
        mat->SetElement(i,3, origin[i]);
    volume->SetUserMatrix(mat);

    //Add coordinate system axes, so we have a reference for position and orientation
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->SetTotalLength(250,250,250);
    axes->SetShaftTypeToCylinder();
    axes->SetCylinderRadius(0.01);
    ren1->AddActor(axes);

    ren1->AddVolume( volume ); 
    ren1->ResetCamera();
  
    renWin->Render();
    renWin->Render();
    renWin->Render();
  
    iren->Start();
  
    return EXIT_SUCCESS;
}
