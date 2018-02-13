#include "itkCovariantVector.h"
#include "itkEdgePotentialImageFilter.h"
#include "itkGradientImageFilter.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

using UnsignedCharImageType = itk::Image< unsigned char, 2 >;

static void CreateImage(UnsignedCharImageType::Pointer image);

int main(int argc, char * argv[])
{
  // Setup types
  using FloatImageType = itk::Image< float,  2 >;
  using VectorImageType = itk::Image< itk::CovariantVector<float, 2>, 2 >;

  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  CreateImage(image);
  
  // Create and setup a gradient filter
  using GradientFilterType = itk::GradientImageFilter<
      UnsignedCharImageType, float>;
  GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
  gradientFilter->SetInput(image);
  gradientFilter->Update();

  // Create and setup an edge potential filter
  using EdgePotentialImageFilterType = itk::EdgePotentialImageFilter<
      VectorImageType, FloatImageType>;
  EdgePotentialImageFilterType::Pointer edgePotentialImageFilter = EdgePotentialImageFilterType::New();
  edgePotentialImageFilter->SetInput( gradientFilter->GetOutput() );
  edgePotentialImageFilter->Update();

  // Scale so we can write to a PNG
  using RescaleFilterType = itk::RescaleIntensityImageFilter< FloatImageType, UnsignedCharImageType >;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(edgePotentialImageFilter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  using FileWriterType = itk::ImageFileWriter<UnsignedCharImageType>;
  FileWriterType::Pointer writer = FileWriterType::New();
  writer->SetFileName("output.png");
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();
  
  return EXIT_SUCCESS;
}


void CreateImage(UnsignedCharImageType::Pointer image)
{
  // Create a black image with 2 white regions
  
  UnsignedCharImageType::IndexType start;
  start.Fill(0);

  UnsignedCharImageType::SizeType size;
  size.Fill(200);

  UnsignedCharImageType::RegionType region(start,size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
  
  // Make a square
  for(unsigned int r = 20; r < 80; r++)
    {
    for(unsigned int c = 30; c < 100; c++)
      {
      UnsignedCharImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 255);

      }
    }

}
