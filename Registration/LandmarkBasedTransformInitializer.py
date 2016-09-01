#!/usr/bin/env python

import itk

Dimension = 2
PixelType = itk.ctype('unsigned char')
ImageType = itk.Image[PixelType, Dimension]

def CreateFixedImage(image):
    start = itk.Index[Dimension]()
    start.Fill(0)

    size = itk.Size[Dimension]()
    size.Fill(100)

    region = itk.ImageRegion[Dimension]()
    region.SetSize(size)
    region.SetIndex(start)

    image.SetRegions(region)
    image.Allocate()
    image.FillBuffer(0)

    index = itk.Index[Dimension]()
    for ii in range(10, 20):
        for jj in range(10, 20):
            index[0] = ii
            index[1] = jj
            image.SetPixel(index, 255)

    writer = itk.ImageFileWriter.New(Input=image)
    writer.SetFileName("fixed.png")
    writer.Update()

def CreateMovingImage(image):
    start = itk.Index[Dimension]()
    start.Fill(0)

    size = itk.Size[Dimension]()
    size.Fill(100)

    region = itk.ImageRegion[Dimension]()
    region.SetSize(size)
    region.SetIndex(start)

    image.SetRegions(region)
    image.Allocate()
    image.FillBuffer(0)

    index = itk.Index[Dimension]()
    for ii in range(50, 60):
        for jj in range(50, 60):
            index[0] = ii
            index[1] = jj
            image.SetPixel(index, 100)

    writer = itk.ImageFileWriter.New(Input=image)
    writer.SetFileName("moving.png")
    writer.Update()

fixed_image = ImageType.New()
CreateFixedImage(fixed_image)

moving_image = ImageType.New()
CreateMovingImage(moving_image)

VectorComponentType = itk.ctype('float')
VectorType = itk.Vector[VectorComponentType, Dimension]
DisplacementFieldType = itk.Image[VectorType, Dimension]

Rigid2DTransformType = itk.Rigid2DTransform[itk.D]
landmark_based_transform_initializer = \
        itk.LandmarkBasedTransformInitializer[itk.Transform[itk.D, Dimension,
            Dimension]].New()

LandmarkPointType = itk.Point[itk.D, Dimension]
LandmarkContainerType = itk.vector[LandmarkPointType]

fixed_landmarks = LandmarkContainerType()
moving_landmarks = LandmarkContainerType()

fixed_point = LandmarkPointType()
moving_point = LandmarkPointType()

fixed_point[0] = 10
fixed_point[1] = 10
moving_point[0] = 50
moving_point[1] = 50
fixed_landmarks.push_back(fixed_point)
moving_landmarks.push_back(moving_point)

fixed_point[0] = 10
fixed_point[1] = 20
moving_point[0] = 50
moving_point[1] = 60
fixed_landmarks.push_back(fixed_point)
moving_landmarks.push_back(moving_point)

fixed_point[0] = 20
fixed_point[1] = 10
moving_point[0] = 60
moving_point[1] = 50
fixed_landmarks.push_back(fixed_point)
moving_landmarks.push_back(moving_point)

fixed_point[0] = 20
fixed_point[1] = 20
moving_point[0] = 60
moving_point[1] = 60
fixed_landmarks.push_back(fixed_point)
moving_landmarks.push_back(moving_point)

landmark_based_transform_initializer.SetFixedLandmarks(fixed_landmarks)
landmark_based_transform_initializer.SetMovingLandmarks(moving_landmarks)

transform = Rigid2DTransformType.New()
transform.SetIdentity()
landmark_based_transform_initializer.SetTransform(transform)
landmark_based_transform_initializer.InitializeTransform()

resampler = itk.ResampleImageFilter.New(Input=moving_image)
resampler.SetTransform(transform)
# resampler.SetReferenceImage(fixed_image)
resampler.SetSize(fixed_image.GetLargestPossibleRegion().GetSize())
resampler.SetOutputOrigin(fixed_image.GetOrigin())
resampler.SetOutputSpacing(fixed_image.GetSpacing())
resampler.SetOutputDirection(fixed_image.GetDirection())
resampler.SetDefaultPixelValue(200)
resampler.UpdateLargestPossibleRegion()

writer = itk.ImageFileWriter.New(Input=resampler.GetOutput())
writer.SetFileName("output.png")
writer.UpdateLargestPossibleRegion()
