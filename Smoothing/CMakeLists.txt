PROJECT (Smoothing)

IF(NOT ITKWikiExamples_BINARY_DIR)
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ENDIF(NOT ITKWikiExamples_BINARY_DIR)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)

#
# Build all .cxx files in the directory
FILE(GLOB ALL_FILES *.cxx)

Requires_Version(RGBBilateralImageFilter 4.0 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT Smoothing)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  BinaryMinMaxCurvatureFlowImageFilter
  BilateralImageFilter
  BinomialBlurImageFilter
  CurvatureFlowImageFilter
  DiscreteGaussianImageFilter
  MeanImageFilter
  MedianImageFilter
  MinMaxCurvatureFlowImageFilter
  RGBCurvatureFlowImageFilter
  RGBMedianImageFilter
  RGBMinMaxCurvatureFlowImageFilter
  SmoothingRecursiveGaussianImageFilter
  VectorCurvatureAnisotropicDiffusionImageFilter
  VectorGradientAnisotropicDiffusionImageFilter
  VectorMeanImageFilter
  VectorMedianImageFilter
)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-BinaryMinMaxCurvatureFlowImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryMinMaxCurvatureFlowImageFilter ${DATA}/Yinyang.png 5)

ADD_TEST(${KIT}-BilateralImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBilateralImageFilter ${DATA}/Gourds.png 4 10)

ADD_TEST(${KIT}-BinomialBlurImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinomialBlurImageFilter ${DATA}/Gourds.png 5)

ADD_TEST(${KIT}-CurvatureFlowImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestCurvatureFlowImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-RGBCurvatureFlowImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBCurvatureFlowImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-DiscreteGaussianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestDiscreteGaussianImageFilter ${DATA}/Gourds.png 9)

ADD_TEST(${KIT}-MinMaxCurvatureFlowImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMinMaxCurvatureFlowImageFilter ${DATA}/Gourds.png 10)

ADD_TEST(${KIT}-RGBMinMaxCurvatureFlowImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBMinMaxCurvatureFlowImageFilter ${DATA}/Gourds.png 10)

ADD_TEST(${KIT}-MeanImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMeanImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-MedianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMedianImageFilter ${DATA}/Gourds.png 3)

ADD_TEST(${KIT}-RGBMedianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBMedianImageFilter ${DATA}/Gourds.png 2)

ADD_TEST(${KIT}-SmoothingRecursiveGaussianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSmoothingRecursiveGaussianImageFilter ${DATA}/Gourds.png 3)

ADD_TEST(${KIT}-VectorGradientAnisotropicDiffusionImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVectorGradientAnisotropicDiffusionImageFilter ${DATA}/Gourds.png 10 1.5)

ADD_TEST(${KIT}-VectorCurvatureAnisotropicDiffusionImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVectorCurvatureAnisotropicDiffusionImageFilter ${DATA}/Gourds.png 10 1.5)

ADD_TEST(${KIT}-VectorMeanImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVectorMeanImageFilter ${DATA}/Gourds.png 4)

ADD_TEST(${KIT}-VectorMedianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVectorMedianImageFilter ${DATA}/Gourds.png 2)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
