#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <itkDenseFrequencyContainer2.h>
#include "itkHistogramToTextureFeaturesFilter.h"
#include "itkScalarImageToCooccurrenceMatrixFilter.h"
#include "itkVectorContainer.h"
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"

//definitions of used types
typedef itk::Image<float, 3> InternalImageType;
typedef itk::Image<unsigned char, 3> VisualizingImageType;
typedef itk::Neighborhood<float, 3> NeighborhoodType;
typedef itk::Statistics::ScalarImageToCooccurrenceMatrixFilter<InternalImageType>
    Image2CoOccuranceType;
typedef Image2CoOccuranceType::HistogramType HistogramType;
typedef itk::Statistics::HistogramToTextureFeaturesFilter<HistogramType> Hist2FeaturesType;
typedef InternalImageType::OffsetType OffsetType;
typedef itk::AddImageFilter <InternalImageType> AddImageFilterType;
typedef itk::MultiplyImageFilter<InternalImageType> MultiplyImageFilterType;

//calculate features for one offset
void calcTextureFeatureImage (OffsetType offset,
    InternalImageType::Pointer inputImage, InternalImageType::Pointer outInertia,
    InternalImageType::Pointer outCorrelation, InternalImageType::Pointer outEnergy)
{
    //allocate output images
    outInertia->CopyInformation(inputImage);
    outInertia->SetRegions(inputImage->GetLargestPossibleRegion());
    outInertia->Allocate();
    outInertia->FillBuffer(0);
    outCorrelation->CopyInformation(inputImage);
    outCorrelation->SetRegions(inputImage->GetLargestPossibleRegion());
    outCorrelation->Allocate();
    outCorrelation->FillBuffer(0);
    outEnergy->CopyInformation(inputImage);
    outEnergy->SetRegions(inputImage->GetLargestPossibleRegion());
    outEnergy->Allocate();
    outEnergy->FillBuffer(0);

    Image2CoOccuranceType::Pointer glcmGenerator=Image2CoOccuranceType::New();
    glcmGenerator->SetOffset(offset);
    glcmGenerator->SetNumberOfBinsPerAxis(16); //reasonable number of bins
    glcmGenerator->SetPixelValueMinMax(0, 255); //for input UCHAR pixel type
    Hist2FeaturesType::Pointer featureCalc=Hist2FeaturesType::New();

    typedef itk::RegionOfInterestImageFilter<InternalImageType,InternalImageType> roiType;
    roiType::Pointer roi=roiType::New();
    roi->SetInput(inputImage);

    InternalImageType::RegionType window;
    InternalImageType::RegionType::SizeType size;
    size.Fill(3); //window size=3x3x3
    window.SetSize(size);
    InternalImageType::IndexType pi; //pixel index
    
    //slide window over the entire image
    for (unsigned x=1; x<inputImage->GetLargestPossibleRegion().GetSize(0)-1; x++)
    {
        pi.SetElement(0,x);
        window.SetIndex(0,x-1);
        for (unsigned y=1; y<inputImage->GetLargestPossibleRegion().GetSize(1)-1; y++)
        {
            pi.SetElement(1,y);
            window.SetIndex(1,y-1);
            for (unsigned z=1; z<inputImage->GetLargestPossibleRegion().GetSize(2)-1; z++)
            {
                pi.SetElement(2,z);
                window.SetIndex(2,z-1);
                roi->SetRegionOfInterest(window);
                roi->Update();
                glcmGenerator->SetInput(roi->GetOutput());
                glcmGenerator->Update();
                featureCalc->SetInput( glcmGenerator->GetOutput() );
                featureCalc->Update();

                outInertia->SetPixel(pi, featureCalc->GetFeature(Hist2FeaturesType::Inertia));
                outCorrelation->SetPixel(pi, featureCalc->GetFeature(Hist2FeaturesType::Correlation));
                outEnergy->SetPixel(pi, featureCalc->GetFeature(Hist2FeaturesType::Energy));
            }
        }
        std::cout<<'.';
    }
}

int main(int, char *[])
{
    typedef itk::ImageFileReader<InternalImageType> ReaderType;
    ReaderType::Pointer reader=ReaderType::New();
    reader->SetFileName("t64.mha"); //http://www.cg.informatik.uni-siegen.de/data/Downloads/t64.mha
    reader->Update();
    InternalImageType::Pointer image=reader->GetOutput();

    NeighborhoodType neighborhood;
    neighborhood.SetRadius(1);
    unsigned int centerIndex = neighborhood.GetCenterNeighborhoodIndex();
    OffsetType offset;

    typedef itk::ImageFileWriter<InternalImageType> WriterType;
    WriterType::Pointer writer=WriterType::New();

    for ( unsigned int d = 0; d < centerIndex; d++ )
    {
        offset = neighborhood.GetOffset(d);
        InternalImageType::Pointer inertia=InternalImageType::New();
        InternalImageType::Pointer correlation=InternalImageType::New();
        InternalImageType::Pointer energy=InternalImageType::New();
        calcTextureFeatureImage(offset, image, inertia, correlation, energy);
        
        writer->SetInput(inertia);
//	snprintf(buf, 100, "Inertia%u.mha", d); // Warning: call to int __builtin___snprintf_chk will always overflow destination buffer
        std::stringstream ssInertia;
        ssInertia << "Inertia" << d << ".mha";
        writer->SetFileName(ssInertia.str());
        writer->Update();
        writer->SetInput(correlation);
        std::stringstream ssCorrelation;
        ssCorrelation << "Correlation" << d << ".mha";
        writer->SetFileName(ssCorrelation.str());
        writer->Update();
        writer->SetInput(energy);
        std::stringstream ssEnergy;
        ssEnergy << "Energy" << d << ".mha";
        writer->SetFileName(ssEnergy.str());
        writer->Update();
        std::cout<<'\n';
    }

    return EXIT_SUCCESS;
}
