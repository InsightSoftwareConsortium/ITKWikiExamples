#include "itkImage.h"
#include "itkNeighborhoodOperatorImageFunction.h"
#include "itkNeighborhoodOperator.h"
#include "itkNeighborhoodAllocator.h"
#include "itkNeighborhoodOperatorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include <itkImageToVTKImageFilter.h>

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"

typedef itk::Image<float, 2>  FloatImageType;
  
void CreateImage(FloatImageType::Pointer image);

class CustomOperator
: public itk::NeighborhoodOperator<float, 2>
{
  // Count the number of valid neighbors and set the output pixel to that number.
  // What we should see is that the number of neighbors is constant in the "middle"
  // of the image and the number of neighbors is fewer on the edges.

  protected:
    CoefficientVector GenerateCoefficients();

    void Fill(const CoefficientVector &coeff)
    {
      //Superclass::FillCenteredDirectional(coeff);
    }
};

CustomOperator::CoefficientVector CustomOperator::GenerateCoefficients()
{
  std::cout << "Neighborhood size: " << this->GetSize() << std::endl;
  /*
  for(unsigned int i = 0; i < this->GetSize(); i++)
    {
    ImageType::PixelType pixel = this->GetElement(i);
    std::cout << "Pixel " << i << " : " << pixel << std::endl;
    }
  */
}

int main(int, char*[])
{

  FloatImageType::Pointer image = FloatImageType::New();
  CreateImage(image);

  CustomOperator customOperator;
  
  itk::NeighborhoodOperatorImageFilter<FloatImageType, FloatImageType>::Pointer filter
        = itk::NeighborhoodOperatorImageFilter<FloatImageType, FloatImageType>::New();
  filter->SetOperator(customOperator);
  filter->SetInput(image);
  filter->Update();
  
  // Visualize
  typedef itk::RescaleIntensityImageFilter< FloatImageType, itk::Image<unsigned char, 2> > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(filter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  
  typedef itk::ImageToVTKImageFilter<itk::Image<unsigned char, 2> > ConnectorType;
  ConnectorType::Pointer connector = ConnectorType::New();
  connector->SetInput(rescaleFilter->GetOutput());

  vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
  actor->SetInput(connector->GetOutput());

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

void CreateImage(FloatImageType::Pointer image)
{
  // Create an image with 2 connected components
  FloatImageType::RegionType region;
  FloatImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  FloatImageType::SizeType size;
  unsigned int NumRows = 200;
  unsigned int NumCols = 300;
  size[0] = NumRows;
  size[1] = NumCols;

  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(unsigned int r = 20; r < 80; r++)
    {
      for(unsigned int c = 20; c < 80; c++)
        {
        FloatImageType::IndexType pixelIndex;
        pixelIndex[0] = r;
        pixelIndex[1] = c;

        image->SetPixel(pixelIndex, 15);
        }
    }
}
