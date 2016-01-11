#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkSigmoidImageFilter.h"

#include "QuickView.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Usage: ";
    std::cerr << argv[0] << " inputImageFile [alpha] [beta]" << std::endl;
    return EXIT_FAILURE;
    }

  double alpha = 5.0;
  double beta = 10.0;
  if (argc > 2)
    {
    alpha = atof(argv[2]);
    }
  if (argc > 3)
    {
    beta = atof(argv[3]);
    }

  typedef itk::Image<unsigned char, 2> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::SigmoidImageFilter <ImageType, ImageType>
    SigmoidImageFilterType;

  SigmoidImageFilterType::Pointer sigmoidFilter
    = SigmoidImageFilterType::New();
  sigmoidFilter->SetInput(reader->GetOutput());
  sigmoidFilter->SetOutputMinimum(0);
  sigmoidFilter->SetOutputMaximum(255);
  sigmoidFilter->SetAlpha(alpha);
  sigmoidFilter->SetBeta(beta);
  
  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "SigmoidImageFilter\nalpha = " << alpha
       << " beta = " << beta;
  viewer.AddImage(
    sigmoidFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
