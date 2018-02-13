#include "itkImage.h"
#include "itkImageFileReader.h"

int main(int argc, char *argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile" << std::endl;
    return EXIT_FAILURE;
    }

  using ImageType = itk::Image<float, 2>;
  using ReaderType = itk::ImageFileReader<ImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  std::cout << reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0] << " "
            << reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1] << std::endl;

  // An example image had w = 200 and h = 100 (it is wider than it is tall). The above output
  // 200 100
  // so w = GetSize()[0]
  // and h = GetSize()[1]

  // A pixel inside the region
  itk::Index<2> indexInside;
  indexInside[0] = 150;
  indexInside[1] = 50;
  std::cout << reader->GetOutput()->GetLargestPossibleRegion().IsInside(indexInside) << std::endl;

  // A pixel outside the region
  itk::Index<2> indexOutside;
  indexOutside[0] = 50;
  indexOutside[1] = 150;
  std::cout << reader->GetOutput()->GetLargestPossibleRegion().IsInside(indexOutside) << std::endl;

  // This means that the [0] component of the index is referencing the left to right (column) index
  // and the [1] component of Index is referencing the top to bottom (row) index

  return EXIT_SUCCESS;
}
