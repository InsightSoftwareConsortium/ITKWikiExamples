PROJECT (EdgesAndGradients)

IF(NOT ITKWikiExamples_BINARY_DIR)
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ENDIF(NOT ITKWikiExamples_BINARY_DIR)

#
# Build all .cxx files in the directory
FILE(GLOB ALL_FILES *.cxx)
foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT EdgesAndGradients)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  DerivativeImageFilter
  GradientMagnitudeImageFilter
  GradientMagnitudeRecursiveGaussianImageFilter
  GradientRecursiveGaussianImageFilter
  LaplacianRecursiveGaussianImageFilter
  RecursiveGaussianImageFilter
  SobelEdgeDetectionImageFilter
  ZeroCrossingBasedEdgeDetectionImageFilter
)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-DerivativeImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestDerivativeImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-GradientMagnitudeImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGradientMagnitudeImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-GradientMagnitudeRecursiveGaussianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGradientMagnitudeRecursiveGaussianImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-GradientRecursiveGaussianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGradientRecursiveGaussianImageFilter ${DATA}/Gourds.png ${TEMP}/GradientRecursiveGaussianImageFilter.mha)

ADD_TEST(${KIT}-LaplacianRecursiveGaussianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLaplacianRecursiveGaussianImageFilter ${DATA}/Yinyang.png)

ADD_TEST(${KIT}-RecursiveGaussianImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRecursiveGaussianImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-SobelEdgeDetectionImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSobelEdgeDetectionImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-ZeroCrossingBasedEdgeDetectionImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestZeroCrossingBasedEdgeDetectionImageFilter ${DATA}/Gourds.png 10)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
