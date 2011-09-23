#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConvolutionImageFilter.h"
#include "itkImageRegionIterator.h"

#include "QuickView.h"

typedef itk::Image<float, 2> ImageType;

static void CreateKernel(ImageType::Pointer kernel, unsigned int width);

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 2 )
    {
    std::cerr << "Usage: ";
    std::cerr << argv[0] << "inputImageFile [width]" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line arguments
  unsigned int width = 3;
  if (argc > 2)
    {
    width = atoi(argv[2]);
    }

  ImageType::Pointer kernel = ImageType::New();
  CreateKernel(kernel, width);
  
  typedef itk::ImageFileReader<ImageType>        ReaderType;
  typedef itk::ConvolutionImageFilter<ImageType> FilterType;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  // Convolve image with kernel.
  FilterType::Pointer convolutionFilter = FilterType::New();
  convolutionFilter->SetInput(reader->GetOutput());
#if ITK_VERSION_MAJOR >= 4
  convolutionFilter->SetImageKernel(kernel);
#else
  convolutionFilter->SetImageKernelInput(kernel);
#endif
  QuickView viewer;
  viewer.AddImage<ImageType>(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "ConvolutionFilter\n"
       << "Kernel Witdh = " << width;
  viewer.AddImage<ImageType>(
    convolutionFilter->GetOutput(),
    true,
    desc.str());
  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateKernel(ImageType::Pointer kernel, unsigned int width)
{
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(width);

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
