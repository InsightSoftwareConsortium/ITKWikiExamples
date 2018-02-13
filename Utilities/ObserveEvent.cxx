#include "itkBinaryNotImageFilter.h"
#include "itkCommand.h"

using ImageType = itk::Image<unsigned char, 2>;
static void CreateImageObserve(ImageType::Pointer image);

using ImageType = itk::Image<unsigned char, 2>;

class MyCommand : public itk::Command
{
  public:
    itkNewMacro( MyCommand );

  public:

    void Execute(itk::Object *caller, const itk::EventObject & event) override
    {
      Execute( (const itk::Object *)caller, event);
    }

    void Execute(const itk::Object * /*object*/, const itk::EventObject & /*event*/) override
    {
      std::cout << "Command called." << std::endl;
    }

};

int main(int, char*[])
{
  ImageType::Pointer image = ImageType::New();
  CreateImageObserve(image);

  using BinaryNotImageFilterType = itk::BinaryNotImageFilter <ImageType>;

  BinaryNotImageFilterType::Pointer filter = BinaryNotImageFilterType::New();
  filter->SetInput(image);

  MyCommand::Pointer myCommand = MyCommand::New();
  filter->AddObserver(itk::ProgressEvent(), myCommand);

  filter->Update();


  return EXIT_SUCCESS;
}

void CreateImageObserve(ImageType::Pointer image)
{
  // Create an image
  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(100);

  ImageType::RegionType region(start, size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

}
