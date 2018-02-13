#include "itkImage.h"
#include "itkRandomImageSource.h"
#include "itkDerivativeImageFilter.h"

int main(int, char *[])
{
  using ImageType = itk::Image<float, 2>;
  
  itk::Size<2> smallSize;
  smallSize.Fill(10);
  
  itk::Index<2> index;
  index.Fill(0);
  
  itk::ImageRegion<2> region(index, smallSize);
   
  itk::Size<2> bigSize;
  bigSize.Fill(10000);
  
  itk::RandomImageSource<ImageType>::Pointer randomImageSource = itk::RandomImageSource<ImageType>::New();
  randomImageSource->SetNumberOfThreads(1); // to produce non-random results
  randomImageSource->SetSize(bigSize);
  randomImageSource->GetOutput()->SetRequestedRegion(smallSize);
  randomImageSource->Update();
  
  std::cout << "Created random image." << std::endl;
  
  using DerivativeImageFilterType = itk::DerivativeImageFilter<ImageType, ImageType >;
  DerivativeImageFilterType::Pointer derivativeFilter = DerivativeImageFilterType::New();
  derivativeFilter->SetInput( randomImageSource->GetOutput() );
  derivativeFilter->SetDirection(0); // "x" axis
  derivativeFilter->GetOutput()->SetRequestedRegion(smallSize);
  derivativeFilter->Update();
  
  std::cout << "Computed derivative." << std::endl;
  
  return EXIT_SUCCESS;
}
