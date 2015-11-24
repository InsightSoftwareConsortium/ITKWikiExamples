#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkCastImageFilter.h"
#include "QuickView.h"

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string fileName = argv[1];

  std::cout << "FileName: " << fileName << std::endl;

  typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;
  typedef itk::Image<float, 2>  FloatImageType;

  typedef itk::ImageFileReader<FloatImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName);

  typedef itk::CastImageFilter< FloatImageType, UnsignedCharImageType > CastFilterType;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(reader->GetOutput());

  QuickView viewer;
  viewer.AddImage<FloatImageType>(reader->GetOutput());
  viewer.AddImage<UnsignedCharImageType>(castFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
