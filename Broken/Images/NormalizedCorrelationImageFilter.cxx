#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkNormalizedCorrelationImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkImageKernelOperator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "QuickView.h"

#include <iostream>
#include <string>

typedef itk::Image<float, 2> FloatImageType;
typedef itk::Image<unsigned char, 2> UnsignedCharImageType;

int main(int argc, char *argv[])
{
  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }
  
  std::string filename = argv[1];
  
  typedef itk::ImageFileReader<FloatImageType> ReaderType;

  // Read the image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename.c_str());
  reader->Update();
  
  // Extract a small region
  typedef itk::RegionOfInterestImageFilter< FloatImageType,
                                            FloatImageType > ExtractFilterType;

  ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();

  FloatImageType::IndexType start;
  start.Fill(50);

  FloatImageType::SizeType size;
  size.Fill(100);

  FloatImageType::RegionType desiredRegion(start,size);

  extractFilter->SetRegionOfInterest(desiredRegion);
  extractFilter->SetInput(reader->GetOutput());
  extractFilter->Update();
  
  // Perform normalized correlation
  // <input type, mask type (not used), output type>
  typedef itk::NormalizedCorrelationImageFilter<FloatImageType, FloatImageType, FloatImageType> CorrelationFilterType;

  itk::ImageKernelOperator<float> kernelOperator;
  kernelOperator.SetImageKernel(extractFilter->GetOutput());
  kernelOperator.CreateToRadius(extractFilter->GetOutput()->GetLargestPossibleRegion().GetSize());
  
  CorrelationFilterType::Pointer correlationFilter = CorrelationFilterType::New();
  correlationFilter->SetInput(reader->GetOutput());
  correlationFilter->SetTemplate(kernelOperator);
  correlationFilter->Update();

  typedef itk::MinimumMaximumImageCalculator <FloatImageType>
          MinimumMaximumImageCalculatorType;

  MinimumMaximumImageCalculatorType::Pointer minimumMaximumImageCalculatorFilter
          = MinimumMaximumImageCalculatorType::New ();
  minimumMaximumImageCalculatorFilter->SetImage(correlationFilter->GetOutput());
  minimumMaximumImageCalculatorFilter->Compute();
  
  std::cout << "Maximum: " << minimumMaximumImageCalculatorFilter->GetIndexOfMaximum() << std::endl;
  std::cout << "Minimum: " << minimumMaximumImageCalculatorFilter->GetIndexOfMinimum() << std::endl;

  typedef itk::RescaleIntensityImageFilter< FloatImageType, UnsignedCharImageType > RescaleFilterType;
  typedef itk::ImageFileWriter<UnsignedCharImageType> WriterType;
  {
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(correlationFilter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(rescaleFilter->GetOutput());
  writer->SetFileName("correlation.png");
  writer->Update();
  }

  {
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(extractFilter->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(rescaleFilter->GetOutput());
  writer->SetFileName("patch.png");
  writer->Update();
  }
  
  QuickView viewer;
  viewer.AddImage(reader->GetOutput());
  viewer.AddImage(extractFilter->GetOutput());
  viewer.AddImage(correlationFilter->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
