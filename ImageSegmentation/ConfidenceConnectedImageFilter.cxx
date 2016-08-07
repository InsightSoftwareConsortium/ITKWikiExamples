#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkConfidenceConnectedImageFilter.h"

#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

typedef itk::Image< unsigned char, 2 >  ImageType;

int main( int argc, char *argv[])
{
  if(argc < 4)
    {
    std::cerr << "Required: filename.png seedX seedY" << std::endl;

    return EXIT_FAILURE;
    }
  std::string inputFileName = argv[1];
  
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName.c_str());
  reader->Update();

  typedef itk::ConfidenceConnectedImageFilter<ImageType, ImageType> ConfidenceConnectedFilterType;
  ConfidenceConnectedFilterType::Pointer confidenceConnectedFilter = ConfidenceConnectedFilterType::New();
  confidenceConnectedFilter->SetInitialNeighborhoodRadius(3);
  confidenceConnectedFilter->SetMultiplier(3);
  confidenceConnectedFilter->SetNumberOfIterations(25);
  confidenceConnectedFilter->SetReplaceValue(255);

  // Set seed
  ImageType::IndexType seed;
  seed[0] = atoi(argv[2]);
  seed[1] = atoi(argv[3]);
  confidenceConnectedFilter->SetSeed(seed);
  confidenceConnectedFilter->SetInput(reader->GetOutput());

  QuickView viewer;
  viewer.AddImage(
    reader->GetOutput(),true,
    itksys::SystemTools::GetFilenameName(inputFileName));  

  std::stringstream desc;
  desc << "ConfidenceConnected Seed: " << seed[0] << ", " << seed[1];
  viewer.AddImage(
    confidenceConnectedFilter->GetOutput(),
    true,
    desc.str());  

  viewer.Visualize();

  return EXIT_SUCCESS;
}
