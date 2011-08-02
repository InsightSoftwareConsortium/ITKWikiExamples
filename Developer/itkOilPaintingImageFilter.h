#ifndef __itkOilPaintingImageFilter_h
#define __itkOilPaintingImageFilter_h
 
#include "itkImageToImageFilter.h"
#include "itkNeighborhoodIterator.h"
 
namespace itk
{
/** \class OilPaintingImageFilter
 * \brief Implements oil painting artistic image filter.
 *
 * Default number of bins is 20.
 * Default radius is 5, meaning a window of 11x11x11 for 3D images.
 *
 * \ingroup ImageFilters
 */
template< class TImage>
class OilPaintingImageFilter:public ImageToImageFilter< TImage, TImage >
{
public:
  /** Standard class typedefs. */
  typedef OilPaintingImageFilter             Self;
  typedef ImageToImageFilter< TImage, TImage > Superclass;
  typedef SmartPointer< Self >        Pointer;
  typedef typename NeighborhoodIterator<TImage>::RadiusType RadiusType;
 
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(OilPaintingImageFilter, ImageToImageFilter);
 
  itkSetMacro(NumberOfBins, unsigned int);
  itkGetConstMacro(NumberOfBins, const unsigned int);
 
  itkSetMacro(Radius, RadiusType);
  itkGetConstMacro(Radius, const RadiusType);
  void SetRadius(unsigned int radius);
 
protected:
  OilPaintingImageFilter();
  ~OilPaintingImageFilter(){}
 
  virtual void BeforeThreadedGenerateData();
 
  /** Does the real work. */
  virtual void ThreadedGenerateData(const typename Superclass::OutputImageRegionType& outputRegionForThread, int threadId);
 
private:
  OilPaintingImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented
 
  typename TImage::ValueType m_Maximum, m_Minimum;
  typename NeighborhoodIterator<TImage>::RadiusType m_Radius;
  unsigned int m_NumberOfBins;
};
} //namespace ITK
 
 
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkOilPaintingImageFilter.txx"
#endif
 
#endif // __itkOilPaintingImageFilter_h
