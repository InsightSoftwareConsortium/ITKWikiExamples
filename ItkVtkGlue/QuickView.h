#ifndef QuickView_H
#define QuickView_H

#include <vector>

#include <itkImage.h>
#include <itkRGBPixel.h>

class ImageInfo
{
public:
  typedef itk::Image<unsigned char, 2> ImageType;

  ImageInfo(ImageType *image, std::string description="")
  {
    m_Image = image;
    m_Description = description;
  }

  ImageType::Pointer m_Image;
  std::string        m_Description;
};

class RGBImageInfo
{
public:
  typedef itk::Image<itk::RGBPixel<unsigned char>, 2> ImageType;
  RGBImageInfo(ImageType *image, std::string description="")
  {
    m_Image = image;
    m_Description = description;
  }

  ImageType::Pointer m_Image;
  std::string        m_Description;
};

class QuickView
{

public:
  QuickView() {m_ShareCamera = true;}
  template<class TImage> void AddImage(
    TImage *,
    bool FlipVertical=true,
    std::string Description="");
  template<class TImage> void AddRGBImage(
    TImage *,
    bool FlipVertical=true,
    std::string Description="");
  void Visualize();
  void ShareCameraOff() {m_ShareCamera = false;}
  void ShareCameraOn()  {m_ShareCamera = true;}
private:
  std::vector<ImageInfo>    Images;
  std::vector<RGBImageInfo> RGBImages;
  bool                      m_ShareCamera;
};

#endif
