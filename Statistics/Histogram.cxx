#include "itkHistogram.h"

int main(int, char *[])
{
  typedef float MeasurementType;
  typedef itk::Statistics::Histogram< MeasurementType,
          itk::Statistics::DenseFrequencyContainer2 > HistogramType;
  HistogramType::Pointer histogram = HistogramType::New();

  unsigned int dimension = 3;
  HistogramType::SizeType size(dimension);
  
  unsigned int binsPerDimension = 30;
  size.Fill(binsPerDimension);
  
  HistogramType::MeasurementVectorType lowerBound;
  lowerBound.SetSize(binsPerDimension);
  lowerBound.Fill(1.1);
  
  HistogramType::MeasurementVectorType upperBound;
  upperBound.SetSize(binsPerDimension);
  upperBound.Fill(6.2);
  
  histogram->SetMeasurementVectorSize(dimension);
  histogram->Initialize(size, lowerBound, upperBound );

  histogram->SetFrequency(0UL, 0.0);
  histogram->SetFrequency(1UL, 2.0);
  histogram->SetFrequency(2UL, 3.0);
  histogram->SetFrequency(3UL, 2.0);
  histogram->SetFrequency(4UL, 0.5);
  histogram->SetFrequency(5UL, 1.0);
  histogram->SetFrequency(6UL, 5.0);
  histogram->SetFrequency(7UL, 2.5);
  histogram->SetFrequency(8UL, 0.0);

  HistogramType::IndexType index(dimension);
  index[0] = 0;
  index[1] = 2;
  index[1] = 4;
  std::cout << "Frequency of the bin at index  " << index
            << " is " << histogram->GetFrequency(index) << std::endl;

	    
  std::cout << "Measurement vector at bin " << index << " is "
            << histogram->GetMeasurementVector(index) << std::endl;
	    
  HistogramType::MeasurementVectorType mv(dimension);
  mv[0] = 4.1;
  mv[1] = 5.6;
  mv[2] = 3.6;

  HistogramType::IndexType resultingIndex;
  histogram->GetIndex(mv, resultingIndex);
  
  std::cout << "Index of the measurement vector " << mv
            << " is " << resultingIndex << std::endl;

  index.Fill(100);
  if ( histogram->IsIndexOutOfBounds(index) )
    {
    std::cout << "Index " << index << " is out of bounds." << std::endl;
    }

  std::cout << "Number of bins = " << histogram->Size()
            << " Total frequency = " << histogram->GetTotalFrequency()
            << " Dimension sizes = " << histogram->GetSize() << std::endl;

  std::cout << "50th percentile along the first dimension = "
            << histogram->Quantile(0, 0.5) << std::endl;

  return EXIT_SUCCESS;
}
