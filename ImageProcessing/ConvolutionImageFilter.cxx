#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkConvolutionImageFilter.h"
#include "itkImageRegionIterator.h"

#include <itkImageToVTKImageFilter.h>

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"

typedef itk::Image<unsigned char, 2> ImageType;

void CreateKernel(ImageType::Pointer kernel);

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
	  std::cerr << "Usage: " << std::endl;
	  std::cerr << argv[0] << "inputImageFile" << std::endl;
	  return EXIT_FAILURE;
    }

  // Parse command line arguments
  std::string inputFilename = argv[1];

  ImageType::Pointer kernel = ImageType::New();
  CreateKernel(kernel);
  
  typedef itk::ImageFileReader<ImageType>  readerType;

  typedef itk::ConvolutionImageFilter<ImageType> FilterType;

  // Create and setup a reader
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );

  // Convolve image with kernel.
  FilterType::Pointer convolutionFilter = FilterType::New();
  convolutionFilter->SetInput(reader->GetOutput());
  convolutionFilter->SetImageKernelInput(kernel);
  convolutionFilter->Update();

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("TestOutput.jpg");
  writer->SetInput(convolutionFilter->GetOutput());
  writer->Update();

  // To visualize the derivative, we must rescale the values
  // to a reasonable range
  typedef itk::RescaleIntensityImageFilter<
		  ImageType, ImageType > rescaleFilterType;

  rescaleFilterType::Pointer rescaler = rescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput( convolutionFilter->GetOutput() );
  rescaler->Update();

  typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
  ConnectorType::Pointer originalConnector = ConnectorType::New();
  originalConnector->SetInput(reader->GetOutput());

  vtkSmartPointer<vtkImageActor> originalActor =
    vtkSmartPointer<vtkImageActor>::New();
  originalActor->SetInput(originalConnector->GetOutput());

  typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
  ConnectorType::Pointer convolvedConnector = ConnectorType::New();
  convolvedConnector->SetInput(rescaler->GetOutput());

  vtkSmartPointer<vtkImageActor> convolvedActor =
    vtkSmartPointer<vtkImageActor>::New();
  convolvedActor->SetInput(convolvedConnector->GetOutput());

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
  rightRenderer->AddActor(convolvedActor);

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

void CreateKernel(ImageType::Pointer kernel)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(3);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  kernel->SetRegions(region);
  kernel->Allocate();
  
  itk::ImageRegionIterator<ImageType> imageIterator(kernel, region);

  while(!imageIterator.IsAtEnd())
    {
    //imageIterator.Set(255);
    imageIterator.Set(1);

    ++imageIterator;
    }
}
