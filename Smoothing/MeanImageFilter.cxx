#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMeanImageFilter.h"
#include "itkSubtractImageFilter.h"

#include "QuickView.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << argv[0] << " InputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  // Setup types
  using UnsignedCharImageType = itk::Image< unsigned char, 2 >;
  using FloatImageType = itk::Image< float, 2 >;
  using ReaderType = itk::ImageFileReader< UnsignedCharImageType >;
  using filterType = itk::MeanImageFilter<
    UnsignedCharImageType, UnsignedCharImageType >;
  using SubtractType = itk::SubtractImageFilter<UnsignedCharImageType, UnsignedCharImageType, FloatImageType>;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  // Create and setup a mean filter
  filterType::Pointer meanFilter = filterType::New();
  meanFilter->SetInput( reader->GetOutput() );

  SubtractType::Pointer diff = SubtractType::New();
  diff->SetInput1(reader->GetOutput());
  diff->SetInput2(meanFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,

    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "Mean";
  viewer.AddImage(
    meanFilter->GetOutput(),
    true,
    desc.str());  

  std::stringstream desc2;
  desc2 << "Original - Mean";
  viewer.AddImage(
    diff->GetOutput(),
    true,
    desc2.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
