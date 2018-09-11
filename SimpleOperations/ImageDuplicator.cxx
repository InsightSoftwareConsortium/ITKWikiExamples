#include "itkImage.h"
#include "itkImageDuplicator.h"
#include "itkRandomImageSource.h"

int main(int, char *[])
{
  using ImageType = itk::Image< unsigned char, 2 >;

  itk::RandomImageSource<ImageType>::Pointer randomImageSource =
    itk::RandomImageSource<ImageType>::New();
  randomImageSource->SetNumberOfWorkUnits(1); // to produce non-random results

  randomImageSource->Update();
  ImageType::Pointer image = randomImageSource->GetOutput();

  using DuplicatorType = itk::ImageDuplicator< ImageType >;
  DuplicatorType::Pointer duplicator = DuplicatorType::New();
  duplicator->SetInputImage(image);
  duplicator->Update();
  ImageType::Pointer clonedImage = duplicator->GetOutput();
  
  return EXIT_SUCCESS;
}
