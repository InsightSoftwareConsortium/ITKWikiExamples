PROJECT (ImageProcessing)

IF(NOT ITKWikiExamples_BINARY_DIR)
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ENDIF(NOT ITKWikiExamples_BINARY_DIR)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)
include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresModule.cmake)

#
# Build all .cxx files in the directory
FILE(GLOB ALL_FILES *.cxx)
Requires_Version(AddConstantToImageFilter 4.0 ALL_FILES)
Requires_Version(BinaryNotImageFilter 4.0 ALL_FILES)
Requires_Version(ClampImageFilter 4.0 ALL_FILES)
Requires_Version(LabelMapOverlayImageFilter 4.0 ALL_FILES)
Requires_Version(LabelStatisticsImageFilter 4.0 ALL_FILES)
Requires_Version(MaskNegatedImageFilter 4.0 ALL_FILES)
Requires_Version(NormalizeToConstantImageFilter 4.0 ALL_FILES)
Requires_Version(ObjectByObjectLabelMapFilter 4.0 ALL_FILES)
Requires_Version(SubtractImageFilter_Constant 4.0 ALL_FILES)

Requires_Module(RegionalMaximaImageFilter Review ALL_FILES)
Requires_Module(RegionalMinimaImageFilter Review ALL_FILES)
Requires_Module(ValuedRegionalMinimalImageFilter Review ALL_FILES)
Requires_Module(LabelGeometryImageFilter Review ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT ImageProcessing)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  ApproximateSignedDistanceMapImageFilter
  BinaryThresholdImageFilter
  CastImageFilter
  ConnectedComponentImageFilter
  ConvolutionImageFilter
  CropImageFilter
  ImageFilterOnARegion
  InvertIntensityImageFilter
  LabelGeometryImageFilter
  LaplacianImageFilter
  LaplacianSharpeningImageFilter
  MaskImageFilter
  MinimumMaximumImageCalculator
  NormalizeImageFilter
  RegionOfInterestImageFilter
  ResampleSegmentedImage
  RescaleIntensityImageFilter
  RGBToLuminanceImageFilter
  RGBResampleImageFilter
  ScalarConnectedComponentImageFilter
  ShapeAttributes
  SigmoidImageFilter
  SignedDanielssonDistanceMapImageFilter
  SignedMaurerDistanceMapImageFilter
  ThresholdImageFilter
  TileImageFilter_SideBySide
  TileImageFilter_CreateVolume
  Upsampling
  ZeroCrossingImageFilter
)
SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-ApproximateSignedDistanceMapImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestApproximateSignedDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-SignedMaurerDistanceMapImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSignedMaurerDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-SignedDanielssonDistanceMapImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSignedDanielssonDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-BinaryThresholdImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryThresholdImageFilter ${DATA}/cthead1.png 100 255)

ADD_TEST(${KIT}-ConnectedComponentImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestConnectedComponentImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-ConvolutionImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestConvolutionImageFilter ${DATA}/cthead1.png 10)

ADD_TEST(${KIT}-CropImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestCropImageFilter ${DATA}/Gourds.png 100 200)

ADD_TEST(${KIT}-ImageFilterOnARegion ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestImageFilterOnARegion ${DATA}/Gourds.png 3)

ADD_TEST(${KIT}-InvertIntensityImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestInvertIntensityImageFilter ${DATA}/Gourds.png 255)

ADD_TEST(${KIT}-MaskImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMaskImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-LabelGeometryImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLabelGeometryImageFilter ${DATA}/ctheadlabels.png ${DATA}/cthead1.png 10)

ADD_TEST(${KIT}-LaplacianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLaplacianImageFilter ${DATA}/cthead1.png)

ADD_TEST(${KIT}-LaplacianSharpeningImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLaplacianSharpeningImageFilter ${DATA}/Pileated.jpg)

ADD_TEST(${KIT}-NormalizeImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestNormalizeImageFilter ${DATA}/cthead1.png)

ADD_TEST(${KIT}-MinimumMaximumImageCalculator ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMinimumMaximumImageCalculator ${DATA}/cthead1.png)

ADD_TEST(${KIT}-RegionOfInterestImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRegionOfInterestImageFilter ${DATA}/Gourds.png 100 200)

ADD_TEST(${KIT}-RescaleIntensityImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRescaleIntensityImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-RGBToLuminanceImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBToLuminanceImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-ResampleSegmentedImage ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestResampleSegmentedImage ${DATA}/BinaryImageWithVariousShapes01.png 2)

ADD_TEST(${KIT}-RGBResampleImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBResampleImageFilter ${DATA}/VHF001.png 10)

ADD_TEST(${KIT}-ScalarConnectedComponentImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestScalarConnectedComponentImageFilter ${DATA}/cthead1.png 4 100)

ADD_TEST(${KIT}-SigmoidImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSigmoidImageFilter ${DATA}/cthead1.png 10 150)

ADD_TEST(${KIT}-ThresholdImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestThresholdImageFilter ${DATA}/cthead1.png 100 150)

ADD_TEST(${KIT}-CastImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestCastImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-TileImageFilter_SideBySide ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestTileImageFilter_SideBySide ${DATA}/Gourds.png ${DATA}/cthead1.png ${TEMP}/TileImageFilter_SideBySide.png)

ADD_TEST(${KIT}-TileImageFilter_CreateVolume ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestTileImageFilter_CreateVolume ${DATA}/Gourds.png ${DATA}/cthead1.png ${TEMP}/TileImageFilter_CreateVolume.mha)

ADD_TEST(${KIT}-Upsampling ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestUpsampling ${DATA}/cthead1.png ${TEMP}/Upsampling.png 512 512)

ADD_TEST(${KIT}-ZeroCrossingImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestZeroCrossingImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-ShapeAttributes ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestShapeAttributes ${DATA}/BinaryImageWithVariousShapes01.png)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
