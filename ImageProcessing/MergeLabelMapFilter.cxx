#include "itkBinaryImageToShapeLabelMapFilter.h"
#include "itkMergeLabelMapFilter.h"

int main(int argc, char* argv[])
{
  typedef itk::Image<int, 3> ImageType;

  //Binary Image to Shape Label Map. 
  typedef itk::BinaryImageToShapeLabelMapFilter<ImageType> BI2SLMType;
  typedef BI2SLMType::OutputImageType LabelMapType;
  typedef BI2SLMType::LabelObjectType LabelObjectType;

  typedef itk::MergeLabelMapFilter<LabelMapType> MergerType;
  MergerType::Pointer merger = MergerType::New();

  int noObjects = 4;

  for (int i = 1; i <= noObjects; i++)
    {
    LabelMapType::Pointer labelMap = LabelMapType::New();
    LabelObjectType::Pointer labelObject = LabelObjectType::New();

    labelObject->SetLabel(1);
    labelMap->AddLabelObject(labelObject);
    labelMap->Update();

    merger->SetInput(i - 1, labelMap);
    }

  merger->Update();
  std::cout << "number of objects:  "
      << merger->GetOutput()->GetNumberOfLabelObjects() << "\n";
  std::cout << "number of expected objects:  " << noObjects << "\n";

  return EXIT_SUCCESS;
}
