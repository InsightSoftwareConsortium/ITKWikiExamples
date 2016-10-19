#include "itkImageFileReader.h"
#include "itkZeroCrossingImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

typedef itk::Image<float, 2>  ImageType;

static void CreateImage(ImageType::Pointer image);

int main(int argc, char *argv[])
{
  ImageType::Pointer image = ImageType::New();
  if (argc < 2)
    {
    CreateImage(image);
    }
  else
    {
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    image = reader->GetOutput();
    }

  typedef  itk::ZeroCrossingImageFilter< ImageType, ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(image);
  filter->SetBackgroundValue(0);
  filter->SetForegroundValue(255);
    
  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true,
    argc > 1 ? itksys::SystemTools::GetFilenameName(argv[1]) : "Generated image");  

  std::stringstream desc;
  desc << "Zero Crossing";
  viewer.AddImage(
    filter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}

void CreateImage(ImageType::Pointer image)
{
  itk::Index<2> start;
  start.Fill(0);

  itk::Size<2> size;
  size.Fill(100);

  itk::ImageRegion<2> region(start,size);
  
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(-1);
  
  // Make half of the image negative
  for(unsigned int i = 0; i < 100; ++i)
    {
    for(unsigned int j = 0; j < 50; ++j)
      {
      itk::Index<2> index;
      index[0] = i;
      index[1] = j;
      image->SetPixel(index, 1);
      }
    }
}
