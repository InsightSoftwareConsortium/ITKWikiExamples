#include "QuickView.h"

#include "itkImage.h"
#include "itkRGBPixel.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkFlipImageFilter.h"

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkActor2D.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkTextProperty.h"
#include "vtkTextMapper.h"

#include "itkImageToVTKImageFilter.h"
#include <algorithm>

typedef itk::Image<itk::RGBPixel<unsigned char>, 2>   UnsignedCharRGBImageType;
typedef itk::Image<itk::RGBPixel<float>, 2>   FloatRGBImageType;

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

template void QuickView::AddImage<CharImageType>(CharImageType *image, bool FlipVertical, std::string Description);
template void QuickView::AddImage<UnsignedShortImageType>(UnsignedShortImageType *image, bool FlipVertical, std::string Description);
template void QuickView::AddImage<ShortImageType>(ShortImageType *image, bool FlipVertical, std::string Description);
template void QuickView::AddImage<UnsignedIntImageType>(UnsignedIntImageType *image, bool FlipVertical, std::string Description);
template void QuickView::AddImage<IntImageType>(IntImageType *image, bool FlipVertical, std::string Description);
template void QuickView::AddImage<UnsignedLongImageType>(UnsignedLongImageType *image, bool FlipVertical, std::string Description);
template void QuickView::AddImage<LongImageType>(LongImageType *image, bool FlipVertical, std::string Description);
template void QuickView::AddImage<FloatImageType>(FloatImageType *image, bool FlipVertical, std::string Description);
template void QuickView::AddImage<DoubleImageType>(DoubleImageType *image, bool FlipVertical, std::string Description);

template< >
void QuickView::AddImage<UnsignedCharImageType>(
  UnsignedCharImageType *image,
  bool FlipVertical,
  std::string Description)
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
    ImageInfo myImage(flipper->GetOutput(), Description);
    this->Images.push_back(myImage);
    }
  else
    {
    ImageInfo myImage(image, Description);
    this->Images.push_back(myImage);
    }
}

template<class TImage>
void QuickView::AddImage(
  TImage *image,
  bool FlipVertical,
  std::string Description)
{
  typedef itk::RescaleIntensityImageFilter<TImage, UnsignedCharImageType > rescaleFilterType;

  typename rescaleFilterType::Pointer rescaler = rescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(image);
  rescaler->Update();
  
  this->AddImage(rescaler->GetOutput(), FlipVertical, Description);
}

template< >
void QuickView::AddRGBImage<UnsignedCharRGBImageType>(
  UnsignedCharRGBImageType *image,
  bool FlipVertical,
  std::string Description)
{
  if (FlipVertical)
    {
    typedef itk::FlipImageFilter< UnsignedCharRGBImageType> FlipFilterType;
    FlipFilterType::Pointer flipper = FlipFilterType::New();
    bool flipAxes[3] = { false, true, false };
    flipper = FlipFilterType::New();
    flipper->SetFlipAxes(flipAxes);
    flipper->SetInput(image);
    flipper->Update();
    RGBImageInfo myImage(flipper->GetOutput());
    this->RGBImages.push_back(myImage);
    }
  else
    {
    RGBImageInfo myImage(image);
    this->RGBImages.push_back(myImage);
    }
}

void QuickView::Visualize()
{
  unsigned int rendererSize = 300;
  unsigned int numberOfImages = this->Images.size() + this->RGBImages.size();
  // Setup the render window and interactor
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(rendererSize * numberOfImages, rendererSize);

  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  // Render all of the images
  double step = 1./(static_cast<double>(numberOfImages));
  std::vector<double*> viewports;

  typedef itk::ImageToVTKImageFilter<itk::Image<unsigned char, 2> >
    ConnectorType;
  typedef itk::ImageToVTKImageFilter<itk::Image<itk::RGBPixel<unsigned char>, 2> >
    RGBConnectorType;
  std::vector<ConnectorType::Pointer> connectors; // Force the connectors to persist (not lose scope) after each iteration of the loop
  std::vector<RGBConnectorType::Pointer>
    RGBconnectors; // Force the connectors to persist after each iteration of the loop

  double background[6] = {.4, .5, .6, .6, .5, .4};

  vtkSmartPointer<vtkCamera> sharedCamera = 
    vtkSmartPointer<vtkCamera>::New();

  for(unsigned int i = 0; i < this->Images.size(); i++)
    {
    ConnectorType::Pointer connector = ConnectorType::New();
    connectors.push_back(connector);
    connector->SetInput(this->Images[i].m_Image);
  
    // (xmin, ymin, xmax, ymax)
    double viewport[4] =
      {static_cast<double>(i)*step, 0.0, static_cast<double>(i+1)*step, 1.0};
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
    if (m_ShareCamera)
      {
      renderer->SetActiveCamera(sharedCamera);
      }
    else
      {
      vtkSmartPointer<vtkCamera> aCamera = 
        vtkSmartPointer<vtkCamera>::New();
      renderer->SetActiveCamera(aCamera);
      }
    std::rotate(background, background + 1, background + 6);

    if (this->Images[i].m_Description != "")
      {
      vtkSmartPointer<vtkTextProperty> textProperty =
        vtkSmartPointer<vtkTextProperty>::New();
      textProperty->SetFontSize(10);
      textProperty->SetFontFamilyToCourier();
      textProperty->SetJustificationToCentered();

      vtkSmartPointer<vtkTextMapper> textMapper =
        vtkSmartPointer<vtkTextMapper>::New();
      textMapper->SetTextProperty(textProperty);
      textMapper->SetInput(this->Images[i].m_Description.c_str());
      
      vtkSmartPointer<vtkActor2D> textActor =
        vtkSmartPointer<vtkActor2D>::New();
      textActor->SetMapper(textMapper);
      textActor->SetPosition(rendererSize/2, 16);
      renderer->AddActor(textActor);
      }

    renderer->AddActor(actor);
    renderer->ResetCamera();
    }

  unsigned int j = 0;
  for(unsigned int i = this->Images.size(); i < this->RGBImages.size(); i++, j++)
    {
    RGBConnectorType::Pointer connector = RGBConnectorType::New();
    RGBconnectors.push_back(connector);
    connector->SetInput(this->RGBImages[j].m_Image);

    // (xmin, ymin, xmax, ymax)
    double viewport[4] =
      {static_cast<double>(i)*step, 0.0, static_cast<double>(i+1)*step, 1.0};
    viewports.push_back(viewport);
    vtkSmartPointer<vtkImageActor> actor =
      vtkSmartPointer<vtkImageActor>::New();
    actor->SetInput(connector->GetOutput());

    // Setup renderer
    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);
    renderer->SetViewport(viewports[j]);
    renderer->SetBackground(background);
    if (m_ShareCamera)
      {
      renderer->SetActiveCamera(sharedCamera);
      }
    else
      {
      vtkSmartPointer<vtkCamera> aCamera = 
        vtkSmartPointer<vtkCamera>::New();
      renderer->SetActiveCamera(aCamera);
      }
    std::rotate(background, background + 1, background + 6);

    if (this->RGBImages[j].m_Description != "")
      {
      vtkSmartPointer<vtkTextProperty> textProperty =
        vtkSmartPointer<vtkTextProperty>::New();
      textProperty->SetFontSize(10);
      textProperty->SetFontFamilyToCourier();
      textProperty->SetJustificationToCentered();

      vtkSmartPointer<vtkTextMapper> textMapper =
        vtkSmartPointer<vtkTextMapper>::New();
      textMapper->SetTextProperty(textProperty);
      textMapper->SetInput(this->RGBImages[j].m_Description.c_str());
      
      vtkSmartPointer<vtkActor2D> textActor =
        vtkSmartPointer<vtkActor2D>::New();
      textActor->SetMapper(textMapper);
      textActor->SetPosition(rendererSize/2, 16);
      renderer->AddActor(textActor);
      }

    renderer->AddActor(actor);
    renderer->ResetCamera();
    }

  renderWindow->Render();

  vtkSmartPointer<vtkInteractorStyleImage> style =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  interactor->SetInteractorStyle(style);
  interactor->Start();
}
