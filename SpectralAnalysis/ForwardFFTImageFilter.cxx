#include "itkCastImageFilter.h"
#include "itkImage.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkForwardFFTImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "QuickView.h"

typedef itk::Image<float, 2> FloatImageType;

static void CreateImage(FloatImageType* const image);

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

  // Extract the real part
  typedef itk::ComplexToRealImageFilter<FFTType::OutputImageType, FloatImageType> RealFilterType;
  RealFilterType::Pointer realFilter = RealFilterType::New();
  realFilter->SetInput(fftFilter->GetOutput());
  realFilter->Update();

  typedef itk::RescaleIntensityImageFilter< FloatImageType, UnsignedCharImageType > RescaleFilterType;
  RescaleFilterType::Pointer realRescaleFilter = RescaleFilterType::New();
  realRescaleFilter->SetInput(realFilter->GetOutput());
  realRescaleFilter->SetOutputMinimum(0);
  realRescaleFilter->SetOutputMaximum(255);
  realRescaleFilter->Update();

  // Extract the imaginary part
  typedef itk::ComplexToImaginaryImageFilter<FFTType::OutputImageType, FloatImageType> ImaginaryFilterType;
  ImaginaryFilterType::Pointer imaginaryFilter = ImaginaryFilterType::New();
  imaginaryFilter->SetInput(fftFilter->GetOutput());
  imaginaryFilter->Update();

  RescaleFilterType::Pointer imaginaryRescaleFilter = RescaleFilterType::New();
  imaginaryRescaleFilter->SetInput(imaginaryFilter->GetOutput());
  imaginaryRescaleFilter->SetOutputMinimum(0);
  imaginaryRescaleFilter->SetOutputMaximum(255);
  imaginaryRescaleFilter->Update();

  // Compute the magnitude
  typedef itk::ComplexToModulusImageFilter<FFTType::OutputImageType, FloatImageType> ModulusFilterType;
  ModulusFilterType::Pointer modulusFilter = ModulusFilterType::New();
  modulusFilter->SetInput(fftFilter->GetOutput());
  modulusFilter->Update();

  RescaleFilterType::Pointer magnitudeRescaleFilter = RescaleFilterType::New();
  magnitudeRescaleFilter->SetInput(modulusFilter->GetOutput());
  magnitudeRescaleFilter->SetOutputMinimum(0);
  magnitudeRescaleFilter->SetOutputMaximum(255);
  magnitudeRescaleFilter->Update();

//   QuickView viewer;
//   viewer.AddImage(image.GetPointer());
//   viewer.AddImage(realRescaleFilter->GetOutput());
//   viewer.AddImage(imaginaryRescaleFilter->GetOutput());
//   viewer.AddImage(magnitudeRescaleFilter->GetOutput());
//   viewer.Visualize();

  // Write the images
  typedef itk::ImageFileWriter<UnsignedCharImageType> WriterType;
  
  WriterType::Pointer realWriter = WriterType::New();
  realWriter->SetFileName("real.png");
  realWriter->SetInput(realRescaleFilter->GetOutput());
  realWriter->Update();
  
  WriterType::Pointer imaginaryWriter = WriterType::New();
  imaginaryWriter->SetFileName("imaginary.png");
  imaginaryWriter->SetInput(imaginaryRescaleFilter->GetOutput());
  imaginaryWriter->Update();
  
  WriterType::Pointer magnitudeWriter = WriterType::New();
  magnitudeWriter->SetFileName("magnitude.png");
  magnitudeWriter->SetInput(magnitudeRescaleFilter->GetOutput());
  magnitudeWriter->Update();
  
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
  for(FloatImageType::IndexValueType r = 40; r < 100; r++)
    {
    for(FloatImageType::IndexValueType c = 40; c < 100; c++)
      {
      FloatImageType::IndexType pixelIndex = {{r,c}};

      image->SetPixel(pixelIndex, 100);
      }
    }
}
