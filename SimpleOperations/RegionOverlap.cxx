#include <cstdlib>
#include <cstdio>

#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"

#include "QuickView.h"

int main( int argc, char *argv[] )
{

  if ( argc < 2)
    {
    std::cerr << argv[0] << ": Please provide an output image name." << std::endl;
    return EXIT_FAILURE;
    }

  const char * out_file_name = argv[1];

  const unsigned int Dimension = 2;
  typedef unsigned char                            ComponentType;
  typedef itk::RGBPixel < ComponentType >          RGBPixelType;
  typedef itk::Image< RGBPixelType, Dimension >    RGBImageType;
  typedef RGBImageType::RegionType                 RegionType;
  typedef RGBImageType::IndexType                  IndexType;
  typedef RGBImageType::SizeType                   SizeType; 
  typedef itk::ImageRegionIterator< RGBImageType > IteratorType;
 
  // Define the region of the entire image. 
  IndexType index = {{0, 0}};
  SizeType size = {{100, 100}};
  RegionType region(index,size);

  // Now define two overlapping regions within that image.
  IndexType indexA = {{9, 9}};
  SizeType sizeA = {{50, 50}};
  RegionType regionA(indexA,sizeA);

  IndexType indexB = {{39, 39}};
  SizeType sizeB = {{50, 50}};
  RegionType regionB(indexB,sizeB);

  // Region C is the intersection of A and B
  // padded by 10 pixels.
  RegionType regionC = regionA;
  regionC.Crop( regionB );
  regionC.PadByRadius( 10 );

  RGBPixelType pix_black(0.0); // Initialize to (0,0,0)

  RGBPixelType pix_red(0.0);
  pix_red.SetRed( 255 );

  RGBPixelType pix_green(0.0);
  pix_green.SetGreen( 255 );

  RGBPixelType pix_blue(0.0);
  pix_blue.SetBlue( 255 );
 
  // A black canvas
  RGBImageType::Pointer image = RGBImageType::New();
  image->SetRegions( region );
  image->Allocate();
  image->FillBuffer( pix_black );

  // Paint region A red.
  IteratorType itA( image, regionA );
  for (itA.GoToBegin(); !itA.IsAtEnd(); ++itA)
    {
    itA.Value() += pix_red;
    }
  // Paint region B green.
  IteratorType itB( image, regionB );
  for (itB.GoToBegin(); !itB.IsAtEnd(); ++itB)
    {
    itB.Value() += pix_green;
    }
  // Paint region C blue.
  IteratorType itC( image, regionC );
  for (itC.GoToBegin(); !itC.IsAtEnd(); ++itC)
    {
    itC.Value() += pix_blue;
    }
  QuickView viewer;
  viewer.AddImage(
    image.GetPointer(),true);

  viewer.Visualize();

  // Writer
  typedef itk::ImageFileWriter< RGBImageType > FileWriterType;
  FileWriterType::Pointer writer = FileWriterType::New();
  writer->SetFileName( out_file_name );
  writer->SetInput( image );

  try
    {
    writer->Update();
    std::cout << "Created " << argv[1] << std::endl;
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
