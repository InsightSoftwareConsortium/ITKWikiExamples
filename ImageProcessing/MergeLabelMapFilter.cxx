#include "itkBinaryImageToShapeLabelMapFilter.h"
#include "itkMergeLabelMapFilter.h"

int main(int, char*[])
{
  using ImageType = itk::Image<int, 3>;

  //Binary Image to Shape Label Map. 
  using BI2SLMType = itk::BinaryImageToShapeLabelMapFilter<ImageType>;
  using LabelMapType = BI2SLMType::OutputImageType;
  using LabelObjectType = BI2SLMType::LabelObjectType;

  using MergerType = itk::MergeLabelMapFilter<LabelMapType>;
  MergerType::Pointer merger = MergerType::New();
  merger->SetMethod(MergerType::PACK);

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
