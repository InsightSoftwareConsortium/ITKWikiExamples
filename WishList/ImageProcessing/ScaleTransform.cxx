#include "itkImage.h"
#include "itkScaleTransform.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"

typedef itk::Image<unsigned char, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImage(image);
  
  std::cout << "input size: " << image->GetLargestPossibleRegion().GetSize() << std::endl;

  std::cout << "Writing input... " << std::endl;
  typedef  itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer inputWriter = WriterType::New();
  inputWriter->SetFileName("input.png");
  inputWriter->SetInput(image);
  inputWriter->Update();

  //typedef itk::ScaleTransform<float, 2> TransformType; // This does not work with 'float'
  typedef itk::ScaleTransform<double, 2> TransformType;
  TransformType::Pointer scaleTransform = TransformType::New();
  itk::FixedArray<float, 2> scale;
  scale[0] = 1.5; // newWidth/oldWidth
  scale[1] = 1.5;
  scaleTransform->SetScale(scale);

  itk::Size<2> newSize;
  newSize[0] = scale[0] * image->GetLargestPossibleRegion().GetSize()[0];
  newSize[1] = scale[1] * image->GetLargestPossibleRegion().GetSize()[1];
  
  itk::Point<float,2> center;
  center[0] = image->GetLargestPossibleRegion().GetSize()[0]/2;
  center[1] = image->GetLargestPossibleRegion().GetSize()[1]/2;
  
  scaleTransform->SetCenter(center);
  
  std::cout << "Resampling... " << std::endl;
  typedef itk::ResampleImageFilter<ImageType, ImageType> ResampleImageFilterType;
  ResampleImageFilterType::Pointer resampleFilter = ResampleImageFilterType::New();
  resampleFilter->SetTransform(scaleTransform);
  resampleFilter->SetInput(image);
  resampleFilter->SetSize(newSize);
  resampleFilter->Update();

  std::cout << "output size: " << resampleFilter->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
  
  std::cout << "Writing output... " << std::endl;
  WriterType::Pointer outputWriter = WriterType::New();
  outputWriter->SetFileName("output.png");
  outputWriter->SetInput(resampleFilter->GetOutput());
  outputWriter->Update();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size;
  size.Fill(100);

  ImageType::RegionType region(start, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  // Make a white square
  for(unsigned int r = 40; r < 60; r++)
    {
    for(unsigned int c = 40; c < 60; c++)
      {
      ImageType::IndexType pixelIndex;
      pixelIndex[0] = r;
      pixelIndex[1] = c;

      image->SetPixel(pixelIndex, 255);
      }
    }
}
