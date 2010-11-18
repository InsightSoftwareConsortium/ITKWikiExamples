#include "QuickView.h"

#include "itkImage.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkFlipImageFilter.h"

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"

#include "itkImageToVTKImageFilter.h"
#include <algorithm>

typedef itk::Image<unsigned char, 2>   UnsignedCharImageType;
typedef itk::Image<char, 2>            CharImageType;
typedef itk::Image<unsigned short, 2>  UnsignedShortImageType;
typedef itk::Image<short, 2>           ShortImageType;
typedef itk::Image<unsigned int, 2>    UnsignedIntImageType;
typedef itk::Image<int, 2>             IntImageType;
typedef itk::Image<unsigned long, 2>   UnsignedLongImageType;
typedef itk::Image<long, 2>            LongImageType;
typedef itk::Image<float, 2>           FloatImageType;
typedef itk::Image<double, 2>          DoubleImageType;

template void QuickView::AddImage<CharImageType>(CharImageType *image, bool FlipVertical);
template void QuickView::AddImage<UnsignedShortImageType>(UnsignedShortImageType *image, bool FlipVertical);
template void QuickView::AddImage<ShortImageType>(ShortImageType *image, bool FlipVertical);
template void QuickView::AddImage<UnsignedIntImageType>(UnsignedIntImageType *image, bool FlipVertical);
template void QuickView::AddImage<IntImageType>(IntImageType *image, bool FlipVertical);
template void QuickView::AddImage<UnsignedLongImageType>(UnsignedLongImageType *image, bool FlipVertical);
template void QuickView::AddImage<LongImageType>(LongImageType *image, bool FlipVertical);
template void QuickView::AddImage<FloatImageType>(FloatImageType *image, bool FlipVertical);
template void QuickView::AddImage<DoubleImageType>(DoubleImageType *image, bool FlipVertical);

template< >
void QuickView::AddImage<UnsignedCharImageType>(UnsignedCharImageType *image, bool FlipVertical)
{
  if (FlipVertical)
    {
    typedef itk::FlipImageFilter< UnsignedCharImageType> FlipFilterType;
    FlipFilterType::Pointer flipper = FlipFilterType::New();
    bool flipAxes[3] = { false, true, false };
    flipper = FlipFilterType::New();
    flipper->SetFlipAxes(flipAxes);
    flipper->SetInput(image);
    flipper->Update();
    this->Images.push_back(flipper->GetOutput());
    }
  else
    {
    this->Images.push_back(image);
    }
}

template<class TImage>
void QuickView::AddImage(TImage *image, bool FlipVertical)
{
  typedef itk::RescaleIntensityImageFilter<TImage, UnsignedCharImageType > rescaleFilterType;

  typename rescaleFilterType::Pointer rescaler = rescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(image);
  rescaler->Update();
  
  this->AddImage(rescaler->GetOutput(), FlipVertical);
}

void QuickView::Visualize()
{
  // Setup the render window
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  unsigned int rendererSize = 300;
  renderWindow->SetSize(rendererSize * this->Images.size(), rendererSize);

  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  // Render all of the images
  double step = 1./(static_cast<double>(this->Images.size()));
  std::vector<double*> viewports;

  typedef itk::ImageToVTKImageFilter<itk::Image<unsigned char, 2> > ConnectorType;
  std::vector<ConnectorType::Pointer> connectors; // Force the connectors to persist (not lose scope) after each iteration of the loop

  double background[6] = {.4, .5, .6, .6, .5, .4};

  vtkSmartPointer<vtkCamera> camera = 
    vtkSmartPointer<vtkCamera>::New();
  for(unsigned int i = 0; i < this->Images.size(); i++)
    {
    ConnectorType::Pointer connector = ConnectorType::New();
    connectors.push_back(connector);
    connector->SetInput(this->Images[i]);
  
    // (xmin, ymin, xmax, ymax)
    double viewport[4] = {static_cast<double>(i)*step, 0.0, static_cast<double>(i+1)*step, 1.0};
    viewports.push_back(viewport);
    vtkSmartPointer<vtkImageActor> actor =
      vtkSmartPointer<vtkImageActor>::New();
    actor->SetInput(connector->GetOutput());

    // Setup renderer
    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);
    renderer->SetViewport(viewports[i]);
    renderer->SetBackground(background);
    renderer->SetActiveCamera(camera);
    std::rotate(background, background + 1, background + 6);

    renderer->AddActor(actor);
    renderer->ResetCamera();
    }

  renderWindow->Render();

  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  interactor->SetInteractorStyle(style);
  interactor->Start();
}
