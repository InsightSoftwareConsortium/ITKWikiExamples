#include <itkFileOutputWindow.h>
#include <itkScaleTransform.h>

int main(int argc, char* argv[])
{
  using myFileOutputWindow = itk::FileOutputWindow;
  myFileOutputWindow::Pointer window = myFileOutputWindow::New();

  if (argc > 1)
    {
    window->SetFileName(argv[1]);
    }
  window->FlushOn();

  // Set the singelton instance
  itk::OutputWindow::SetInstance(window);

  // Generic output
  itkGenericOutputMacro("This should be in the file: " << window->GetFileName());
  // Warning
  using TransformType = itk::ScaleTransform<float,2>;
  TransformType::Pointer transform = TransformType::New();
  TransformType::FixedParametersType parameters(3);
  transform->SetFixedParameters(parameters);

  std::cout << "Look in " << window->GetFileName() << " for the output" << std::endl;
  return EXIT_SUCCESS;
}
