#include "itkImage.h"
#include "itkForwardFFTImageFilter.h"
#include "itkInverseFFTImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileReader.h"
#include "itkCastImageFilter.h"
#include "itkImageFileWriter.h"

#include "QuickView.h"

typedef itk::Image<float, 2> FloatImageType;
  
void CreateImage(FloatImageType* const image);

int main(int argc, char*argv[])
{
  FloatImageType::Pointer image;
  // Verify input
  if(argc < 2)
    {
    image = FloatImageType::New();
    CreateImage(image);
    //std::cerr << "Required: filename" << std::endl;
    //return EXIT_FAILURE;
    }
  else
  {
    // Read the image
    typedef itk::ImageFileReader<FloatImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();

    image = reader->GetOutput();
  }

  // Define some types
  typedef itk::Image<unsigned char, 2> UnsignedCharImageType;

  // Compute the FFT
  typedef itk::ForwardFFTImageFilter<FloatImageType> FFTType;
  FFTType::Pointer fftFilter = FFTType::New();
  fftFilter->SetInput(image);
  fftFilter->Update();

  // Compute the IFFT
  //typedef itk::InverseFFTImageFilter<FFTType::OutputImageType, UnsignedCharImageType> IFFTType; // This does not work - output type seems to need to be float, but it is just an error, not a concept check error...
  typedef itk::InverseFFTImageFilter<FFTType::OutputImageType, FloatImageType> IFFTType;
  IFFTType::Pointer ifftFilter = IFFTType::New();
  ifftFilter->SetInput(fftFilter->GetOutput());
  ifftFilter->Update();

//   QuickView viewer;
//   viewer.AddImage(image.GetPointer());
//   viewer.AddImage(ifftFilter->GetOutput());
//   viewer.Visualize();

  typedef itk::CastImageFilter< FloatImageType, UnsignedCharImageType > CastFilterType;
  CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput(ifftFilter->GetOutput());
  castFilter->Update();
  
  typedef itk::ImageFileWriter<UnsignedCharImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("ifft.png");
  writer->SetInput(castFilter->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}

void CreateImage(FloatImageType* const image)
{
  itk::Index<2> corner = {{0,0}};

  itk::Size<2> size = {{200,200}};

  itk::ImageRegion<2> region(corner, size);

  image->SetRegions(region);
  image->Allocate();

  // Make a square
  for(unsigned int r = 40; r < 100; r++)
    {
    for(unsigned int c = 40; c < 100; c++)
      {
      FloatImageType::IndexType pixelIndex = {{r,c}};

      image->SetPixel(pixelIndex, 100);
      }
    }
}
