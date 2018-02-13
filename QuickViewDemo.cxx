#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"

#include "QuickView.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  using ImageType = itk::Image<unsigned char, 2>;

  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  using RescaleFilterType = itk::RescaleIntensityImageFilter< ImageType, ImageType >;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(reader->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  QuickView viewer;
  viewer.AddImage(reader->GetOutput());
  viewer.AddImage(rescaleFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
