#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkDanielssonDistanceMapImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;
typedef itk::Image<float, 2>          FloatImageType;
 
static void CreateImage(UnsignedCharImageType::Pointer image);

int main(int argc, char * argv[])
{
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  if (argc < 2)
    {
    CreateImage(image);
    }
  else
    {
    typedef itk::ImageFileReader<UnsignedCharImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    image = reader->GetOutput();
    }

  typedef  itk::DanielssonDistanceMapImageFilter< UnsignedCharImageType, FloatImageType  > DanielssonDistanceMapImageFilterType;
  DanielssonDistanceMapImageFilterType::Pointer distanceMapImageFilter =
    DanielssonDistanceMapImageFilterType::New();
  distanceMapImageFilter->SetInput(image);
  distanceMapImageFilter->InputIsBinaryOn();
  
  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");  

  std::stringstream desc;
  desc << "Danielsson Distance";
  viewer.AddImage(
    distanceMapImageFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}


void CreateImage(UnsignedCharImageType::Pointer image)
{
  // Create an image
  itk::Index<2> start;
  start.Fill(0);
 
  itk::Size<2> size;
  size.Fill(100);
 
  itk::ImageRegion<2> region(start, size);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);
 
  // Create a line of white pixels
  for(unsigned int i = 40; i < 60; ++i)
    {
    itk::Index<2> pixel;
    pixel.Fill(i);
    image->SetPixel(pixel, 255);
    }

}
