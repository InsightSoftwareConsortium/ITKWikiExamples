#include "itkImage.h"
#include "itkNeighborhoodOperatorImageFilter.h"
#include "itkSobelOperator.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

using FloatImageType = itk::Image<float, 2>;

void CreateImage(FloatImageType::Pointer image);
void CastRescaleAndWrite(FloatImageType::Pointer image, const std::string& filename);

int main(int, char*[])
{
  FloatImageType::Pointer image = FloatImageType::New();
  CreateImage(image);
  CastRescaleAndWrite(image, "input.png");
  
  using SobelOperatorType = itk::SobelOperator<float, 2>;
  SobelOperatorType sobelOperator;
  itk::Size<2> radius;
  radius.Fill(1); // a radius of 1x1 creates a 3x3 operator
  sobelOperator.SetDirection(0); // Create the operator for the X axis derivative
  sobelOperator.CreateToRadius(radius);
  
  using NeighborhoodOperatorImageFilterType = itk::NeighborhoodOperatorImageFilter<FloatImageType, FloatImageType>;
  NeighborhoodOperatorImageFilterType::Pointer filter = NeighborhoodOperatorImageFilterType::New();
  filter->SetOperator(sobelOperator);
  filter->SetInput(image);
  filter->Update();

  CastRescaleAndWrite(filter->GetOutput(), "output.png");
  
  return EXIT_SUCCESS;
}

void CreateImage(FloatImageType::Pointer image)
{
  FloatImageType::IndexType start;
  start.Fill(0);

  FloatImageType::SizeType size;
  size.Fill(100);

  FloatImageType::RegionType region(start,size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // Make a square
  for(unsigned int r = 20; r < 80; r++)
    {
    for(unsigned int c = 20; c < 80; c++)
      {
      FloatImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 15);
      }
    }
}

void CastRescaleAndWrite(FloatImageType::Pointer image, const std::string& filename)
{
  using UnsignedCharImageType = itk::Image<unsigned char, 2>;
  using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, UnsignedCharImageType>;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(image);
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();
  
  using WriterType = itk::ImageFileWriter<UnsignedCharImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(filename);
  writer->SetInput(rescaleFilter->GetOutput());
  writer->Update();

}
