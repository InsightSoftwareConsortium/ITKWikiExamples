#ifndef QuickView_H
#define QuickView_H

#include <vector>

#include <itkImage.h>

class QuickView
{

public:
  template<class TImage> void AddImage(TImage *, bool FlipVertical=true);
  void Visualize();

private:
  std::vector<itk::Image<unsigned char, 2>::Pointer > Images;
};

#endif
