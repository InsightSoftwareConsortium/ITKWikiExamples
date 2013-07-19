#include "itkImage.h"
#if ITK_VERSION_MAJOR < 4
#include "itkVnlFFTRealToComplexConjugateImageFilter.h"
#else
#include "itkVnlForwardFFTImageFilter.h"
#endif
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkImageFileReader.h"
#include "itkPasteImageFilter.h"

#include <itksys/SystemTools.hxx>
#include "vnl/vnl_sample.h"
#include <math.h>

#include <itkImageToVTKImageFilter.h>

#include "QuickView.h"

int main(int argc, char*argv[])
{
  // Verify input
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }

  // Define some types
  typedef itk::Image<float, 2> FloatImageType;

  // Read the image
  typedef itk::ImageFileReader<FloatImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  // Compute the smallest power of two that is bigger than the image
  unsigned int powerOfTwo = 0;
  for(unsigned int i = 0; i < 20; i++)
    {
    if(pow(2.0, static_cast<double>(i)) >= reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0] &&
       pow(2.0, static_cast<double>(i)) >= reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1])
      {
      powerOfTwo = i;
      break;
      }
    }

  // Create an image bigger than the input image and that has
  // dimensions which are powers of two
  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size;
  size.Fill(pow(2.0,static_cast<double>(powerOfTwo)));

  itk::ImageRegion<2> region(start, size);

  FloatImageType::Pointer image = FloatImageType::New();
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // The image dimensions must be powers of two
  typedef itk::PasteImageFilter <FloatImageType, FloatImageType >
    PasteImageFilterType;
  itk::Index<2> destinationIndex;
  destinationIndex.Fill(0);
  
  PasteImageFilterType::Pointer pasteFilter
    = PasteImageFilterType::New ();
  pasteFilter->SetSourceImage(reader->GetOutput());
  pasteFilter->SetDestinationImage(image);
  pasteFilter->SetSourceRegion(reader->GetOutput()->GetLargestPossibleRegion());
  pasteFilter->SetDestinationIndex(destinationIndex);
  pasteFilter->Update();
  
  image->Graft(pasteFilter->GetOutput());
  
  // Compute the FFT
#if ITK_VERSION_MAJOR < 4
  typedef itk::VnlFFTRealToComplexConjugateImageFilter<FloatImageType> FFTType;
#else
  typedef itk::VnlForwardFFTImageFilter<FloatImageType> FFTType;
#endif
  FFTType::Pointer fftFilter = FFTType::New();
  fftFilter->SetInput(image);
  
  // Extract the real part
  typedef itk::ComplexToRealImageFilter<FFTType::OutputImageType, FloatImageType> RealFilterType;
  RealFilterType::Pointer realFilter = RealFilterType::New();
  realFilter->SetInput(fftFilter->GetOutput());

  // Extract the complex part
  typedef itk::ComplexToImaginaryImageFilter<FFTType::OutputImageType, FloatImageType> ImaginaryFilterType;
  ImaginaryFilterType::Pointer imaginaryFilter = ImaginaryFilterType::New();
  imaginaryFilter->SetInput(fftFilter->GetOutput());

  // Compute the magnitude
  typedef itk::ComplexToModulusImageFilter<FFTType::OutputImageType, FloatImageType> ModulusFilterType;
  ModulusFilterType::Pointer modulusFilter = ModulusFilterType::New();
  modulusFilter->SetInput(fftFilter->GetOutput());

  QuickView viewer;
  viewer.AddImage(image.GetPointer(), true,
    itksys::SystemTools::GetFilenameName(argv[1]));
  viewer.AddImage(realFilter->GetOutput(), true,
    "Real");
  viewer.AddImage(imaginaryFilter->GetOutput(), true,
    "Imaginary");
  viewer.AddImage(modulusFilter->GetOutput(), true,
    "Magnitude");
  viewer.Visualize();

  return EXIT_SUCCESS;
}
