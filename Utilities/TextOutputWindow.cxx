#include <itkTextOutput.h>

int main(int, char*[])
{
  // Force output to stdout
  // On Windows systems, the output of warnings is normally sent to a
  // window and not the shell
  itk::OutputWindow::SetInstance(itk::TextOutput::New());

  itkGenericOutputMacro(<< "This should appear in the console window, not in a pop up window");

  return EXIT_SUCCESS;
}
