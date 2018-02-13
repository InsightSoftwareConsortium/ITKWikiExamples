#include "itkMedianImageFilter.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkSubtractImageFilter.h"
#include "itkRGBPixel.h"
#include "itkCastImageFilter.h"

#include "itksys/SystemTools.hxx"

#include <sstream>

#include "QuickView.h"

namespace itk
{
/** \class myRGBPixel
 * \brief Extends RGBPixel with operator <=
 *
 * This class overrides the <= and < operators to use Luminance as a sorting value.
 */
template< typename TComponent = unsigned short >
class myRGBPixel : public RGBPixel<TComponent>
{
public:
  using Self = myRGBPixel;
  using Superclass = RGBPixel<TComponent>;
  using RGBPixel<TComponent>::operator=;

  bool operator<=(const Self & r) const
  {
    if (this->GetLuminance() <= r.GetLuminance())
      {
      return true;
      }
    else
      {
      return false;
      }
  }
  bool operator<(const Self & r) const
  {
    if (this->GetLuminance() < r.GetLuminance())
      {
      return true;
      }
    else
      {
      return false;
      }
  }
};
}

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImageFile [radius]" << std::endl;
    return EXIT_FAILURE;
    }
  std::string inputFilename = argv[1];

  // Setup types
  using PixelType = itk::myRGBPixel<unsigned char>;
  using ImageType = itk::Image<PixelType, 2 >;
  using RGBPixelType = itk::RGBPixel<unsigned char>;
  using RGBImageType = itk::Image<RGBPixelType, 2 >;

  using ReaderType = itk::ImageFileReader<ImageType>;
  using FilterType = itk::MedianImageFilter<ImageType, ImageType >;
  using CastType = itk::CastImageFilter<ImageType, RGBImageType>;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename );

  // Create and setup a median filter
  FilterType::Pointer medianFilter = FilterType::New();
  FilterType::InputSizeType radius;
  radius.Fill(2);
  if (argc > 2)
    {
    radius.Fill(atoi(argv[2]));
    }

  medianFilter->SetRadius(radius);
  medianFilter->SetInput(reader->GetOutput());

  // Cast the custom myRBGPixel's to RGBPixel's
  CastType::Pointer castReader = CastType::New();
  castReader->SetInput(reader->GetOutput());

  CastType::Pointer castMedian = CastType::New();
  castMedian->SetInput(medianFilter->GetOutput());

  QuickView viewer;
  viewer.AddRGBImage(
    castReader->GetOutput(),
    true,
    itksys::SystemTools::GetFilenameName(inputFilename));  

  std::stringstream desc;
  desc << "MedianImageFilter, radius = " << radius;
  viewer.AddRGBImage(
    castMedian->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
