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
  LaplacianImageFilter
  LaplacianSharpeningImageFilter
  MaskImageFilter
  MinimumMaximumImageCalculator
  NormalizeImageFilter
  RegionOfInterestImageFilter
  RescaleIntensityImageFilter
  RGBToLuminanceImageFilter
  RGBResampleImageFilter
  ScalarConnectedComponentImageFilter
  ShapeAttributes
  SigmoidImageFilter
  SignedDaniellsonDistanceMapImageFilter
  SignedMaurerDistanceMapImageFilter
  ThresholdImageFilter
  TileImageFilter_SideBySide
  TileImageFilter_CreateVolume
  Upsampling
  ZeroCrossingImageFilter
)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-ApproximateSignedDistanceMapImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestApproximateSignedDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-SignedMaurerDistanceMapImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestSignedMaurerDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-SignedDanielssonDistanceMapImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestSignedDanielssonDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-BinaryThresholdImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestBinaryThresholdImageFilter ${DATA}/cthead1.png 100 255)

ADD_TEST(${KIT}-ConnectedComponentImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestConnectedComponentImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-ConvolutionImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestConvolutionImageFilter ${DATA}/cthead1.png 10)

ADD_TEST(${KIT}-CropImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestCropImageFilter ${DATA}/Gourds.png 100 200)

ADD_TEST(${KIT}-ImageFilterOnARegion ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestImageFilterOnARegion ${DATA}/Gourds.png 3)

ADD_TEST(${KIT}-InvertIntensityImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestInvertIntensityImageFilter ${DATA}/Gourds.png 255)

ADD_TEST(${KIT}-MaskImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestMaskImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-LaplacianImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestLaplacianImageFilter ${DATA}/cthead1.png)

ADD_TEST(${KIT}-LaplacianSharpeningImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestLaplacianSharpeningImageFilter ${DATA}/Pileated.jpg)

ADD_TEST(${KIT}-NormalizeImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestNormalizeImageFilter ${DATA}/cthead1.png)

ADD_TEST(${KIT}-MinimumMaximumImageCalculator ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestMinimumMaximumImageCalculator ${DATA}/cthead1.png)

ADD_TEST(${KIT}-RegionOfInterestImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestRegionOfInterestImageFilter ${DATA}/Gourds.png 100 200)

ADD_TEST(${KIT}-RescaleIntensityImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestRescaleIntensityImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-RGBToLuminanceImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestRGBToLuminanceImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-RGBResampleImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestRGBResampleImageFilter ${DATA}/VHF001.png 10)

ADD_TEST(${KIT}-ScalarConnectedComponentImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestScalarConnectedComponentImageFilter ${DATA}/cthead1.png 4 100)

ADD_TEST(${KIT}-SigmoidImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestSigmoidImageFilter ${DATA}/cthead1.png 10 150)

ADD_TEST(${KIT}-ThresholdImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestThresholdImageFilter ${DATA}/cthead1.png 100 150)

ADD_TEST(${KIT}-CastImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestCastImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-TileImageFilter_SideBySide ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestTileImageFilter_SideBySide ${DATA}/Gourds.png ${DATA}/cthead1.png ${TEMP}/TileImageFilter_SideBySide.png)

ADD_TEST(${KIT}-TileImageFilter_CreateVolume ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestTileImageFilter_CreateVolume ${DATA}/Gourds.png ${DATA}/cthead1.png ${TEMP}/TileImageFilter_CreateVolume.mha)

ADD_TEST(${KIT}-Upsampling ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestUpsampling ${DATA}/cthead1.png ${TEMP}/Upsampling.png 512 512)

ADD_TEST(${KIT}-ZeroCrossingImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestZeroCrossingImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-ShapeAttributes ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestShapeAttributes ${DATA}/BinaryImageWithVariousShapes01.png)

endif()
