#include "itkImage.h"
#include "itkRandomImageSource.h"

#include "QuickView.h"

int main(int, char *[])
{
  using ImageType = itk::Image< unsigned char, 2 >;

  itk::Size<2> size;
  size.Fill(10);

  itk::RandomImageSource<ImageType>::Pointer randomImageSource =
    itk::RandomImageSource<ImageType>::New();
  randomImageSource->SetNumberOfWorkUnits(1); // to produce non-random results
  randomImageSource->SetSize(size);
  randomImageSource->Update();

  QuickView viewer;
  viewer.AddImage(randomImageSource->GetOutput());
  viewer.Visualize();

  return EXIT_SUCCESS;
}
