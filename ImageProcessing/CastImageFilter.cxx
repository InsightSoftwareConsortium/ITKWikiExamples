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

  using UnsignedCharImageType = itk::Image<unsigned char, 2>;
  using FloatImageType = itk::Image<float, 2>;

  using ReaderType = itk::ImageFileReader<FloatImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName);

  using CastFilterType = itk::CastImageFilter< FloatImageType, UnsignedCharImageType >;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(reader->GetOutput());

  QuickView viewer;
  viewer.AddImage<FloatImageType>(reader->GetOutput());
  viewer.AddImage<UnsignedCharImageType>(castFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
