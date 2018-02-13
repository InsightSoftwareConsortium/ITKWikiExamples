#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkSignedDanielssonDistanceMapImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

using UnsignedCharImageType = itk::Image<unsigned char, 2>;
using FloatImageType = itk::Image<float, 2>;
 
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
    using ReaderType = itk::ImageFileReader<UnsignedCharImageType>;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    image = reader->GetOutput();
    }

  using SignedDanielssonDistanceMapImageFilterType = itk::SignedDanielssonDistanceMapImageFilter< UnsignedCharImageType, FloatImageType  >;
  SignedDanielssonDistanceMapImageFilterType::Pointer distanceMapImageFilter =
    SignedDanielssonDistanceMapImageFilterType::New();
  distanceMapImageFilter->SetInput(image);
  
  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");  

  std::stringstream desc;
  desc << "Signed Danielsson Distance";
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
