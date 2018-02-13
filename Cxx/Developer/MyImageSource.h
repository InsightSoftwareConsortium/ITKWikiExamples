#ifndef __MyImageSource_h
#define __MyImageSource_h

#include "itkImageSource.h"

#include <set>

namespace itk
{
template< class TImage>
class MyImageSource:public ImageSource< TImage >
{
public:

  /** Standard class type alias. */
  using Self = MyImageSource;
  using Superclass = ImageSource< TImage >;
  using Pointer = SmartPointer< Self >;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MyImageSource, ImageSource);

protected:
  MyImageSource(){}
  ~MyImageSource(){}

  /** Does the real work. */
  virtual void GenerateData();

private:
  MyImageSource(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "MyImageSource.txx"
#endif

#endif // __MyImageSource_h
