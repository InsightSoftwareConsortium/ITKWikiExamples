project (ImageProcessing)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
endif()

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)
include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresModule.cmake)

#
# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)
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
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach()

if(BUILD_TESTING)
# Testing
set(KIT ImageProcessing)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
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
set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-ApproximateSignedDistanceMapImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestApproximateSignedDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

add_test(${KIT}-SignedMaurerDistanceMapImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSignedMaurerDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

add_test(${KIT}-SignedDanielssonDistanceMapImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSignedDanielssonDistanceMapImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

add_test(${KIT}-BinaryThresholdImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryThresholdImageFilter ${DATA}/cthead1.png 100 255)

add_test(${KIT}-ConnectedComponentImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestConnectedComponentImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

add_test(${KIT}-ConvolutionImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestConvolutionImageFilter ${DATA}/cthead1.png 10)

add_test(${KIT}-CropImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestCropImageFilter ${DATA}/Gourds.png 100 200)

add_test(${KIT}-ImageFilterOnARegion ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestImageFilterOnARegion ${DATA}/Gourds.png 3)

add_test(${KIT}-InvertIntensityImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestInvertIntensityImageFilter ${DATA}/Gourds.png 255)

add_test(${KIT}-MaskImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMaskImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-LabelGeometryImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLabelGeometryImageFilter ${DATA}/ctheadlabels.png ${DATA}/cthead1.png 10)

add_test(${KIT}-LaplacianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLaplacianImageFilter ${DATA}/cthead1.png)

add_test(${KIT}-LaplacianSharpeningImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLaplacianSharpeningImageFilter ${DATA}/Pileated.jpg)

add_test(${KIT}-NormalizeImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestNormalizeImageFilter ${DATA}/cthead1.png)

add_test(${KIT}-MinimumMaximumImageCalculator ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMinimumMaximumImageCalculator ${DATA}/cthead1.png)

add_test(${KIT}-RegionOfInterestImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRegionOfInterestImageFilter ${DATA}/Gourds.png 100 200)

add_test(${KIT}-RescaleIntensityImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRescaleIntensityImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-RGBToLuminanceImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBToLuminanceImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-ResampleSegmentedImage ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestResampleSegmentedImage ${DATA}/BinaryImageWithVariousShapes01.png 2)

add_test(${KIT}-RGBResampleImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBResampleImageFilter ${DATA}/VHF001.png 10)

add_test(${KIT}-ScalarConnectedComponentImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestScalarConnectedComponentImageFilter ${DATA}/cthead1.png 4 100)

add_test(${KIT}-SigmoidImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSigmoidImageFilter ${DATA}/cthead1.png 10 150)

add_test(${KIT}-ThresholdImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestThresholdImageFilter ${DATA}/cthead1.png 100 150)

add_test(${KIT}-CastImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestCastImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-TileImageFilter_SideBySide ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestTileImageFilter_SideBySide ${DATA}/Gourds.png ${DATA}/cthead1.png ${TEMP}/TileImageFilter_SideBySide.png)

add_test(${KIT}-TileImageFilter_CreateVolume ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestTileImageFilter_CreateVolume ${DATA}/Gourds.png ${DATA}/cthead1.png ${TEMP}/TileImageFilter_CreateVolume.mha)

add_test(${KIT}-Upsampling ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestUpsampling ${DATA}/cthead1.png ${TEMP}/Upsampling.png 512 512)

add_test(${KIT}-ZeroCrossingImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestZeroCrossingImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

add_test(${KIT}-ShapeAttributes ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestShapeAttributes ${DATA}/BinaryImageWithVariousShapes01.png)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
