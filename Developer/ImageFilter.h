#ifndef __itkImageFilter_h
#define __itkImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkRecursiveGaussianImageFilter.h"

namespace itk
{
template< class TImage>
class ImageFilter:public ImageToImageFilter< TImage, TImage >
{
public:
  /** Standard class typedefs. */
  typedef ImageFilter             Self;
  typedef ImageToImageFilter< TImage, TImage > Superclass;
  typedef SmartPointer< Self >        Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFilter, ImageToImageFilter);


  /** Image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);


  /**  Smoothing filter type */
  typedef RecursiveGaussianImageFilter<
    TImage,
    TImage
    >    InternalGaussianFilterType;

  /**  Pointer to a gaussian filter.  */
  typedef typename InternalGaussianFilterType::Pointer InternalGaussianFilterPointer;

protected:
  ImageFilter();
  ~ImageFilter(){}

  /** Does the real work. */
  virtual void GenerateData();

private:
  ImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "ImageFilter.hxx"
#endif


#endif // __itkImageFilter_h
