#ifndef QuickView_H
#define QuickView_H

#include <vector>

#include <itkImage.h>
#include <itkRGBPixel.h>

class QuickView
{

public:
  template<class TImage> void AddImage(TImage *, bool FlipVertical=true);
  template<class TImage> void AddRGBImage(TImage *, bool FlipVertical=true);
  void Visualize();

private:
  std::vector<itk::Image<unsigned char, 2>::Pointer >                Images;
  std::vector<itk::Image<itk::RGBPixel<unsigned char>, 2>::Pointer > RGBImages;
};

#endif
