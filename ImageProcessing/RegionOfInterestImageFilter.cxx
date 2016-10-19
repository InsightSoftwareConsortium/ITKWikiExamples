#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkRGBPixel.h"

#include "QuickView.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Usage: ";
    std::cerr << argv[0] << " inputImageFile [start] [size]" << std::endl;
    return EXIT_FAILURE;
    }

  std::string filename = argv[1];

  typedef itk::Image<itk::RGBPixel<unsigned char>, 2> ImageType;
  typedef itk::ImageFileReader<ImageType>             ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename.c_str());
  reader->Update();

  ImageType::SizeType inSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();

  typedef itk::RegionOfInterestImageFilter< ImageType, ImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();

  ImageType::IndexType start;
  start[0] = inSize[0]/4;
  start[1] = inSize[1]/4;

  ImageType::SizeType size;
  size[0] = inSize[0]/4;
  size[1] = inSize[1]/4;

  if (argc > 2)
    {
    start.Fill(atoi(argv[2]));
    }
  if (argc > 3)
    {
    size.Fill(atoi(argv[3]));
    }


  ImageType::RegionType desiredRegion;
  desiredRegion.SetSize(size);
  desiredRegion.SetIndex(start);

  filter->SetRegionOfInterest(desiredRegion);
  filter->SetInput(reader->GetOutput());

  QuickView viewer;
  viewer.AddRGBImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(argv[1]));  

  std::stringstream desc;
  desc << "RegionOfInterestImageFilter\nstart = " << start
       << " size = " << size;
  viewer.AddRGBImage(
    filter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
