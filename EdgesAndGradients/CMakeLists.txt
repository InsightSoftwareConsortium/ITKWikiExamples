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
  ADD_EXECUTABLE(${EXAMPLE} ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${EXAMPLE} ItkVtkGlue)
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT EdgesAndGradients)
SET(KIT_LIBS ItkVtkGlue)
SET(NEEDS_ARGS
  DerivativeImageFilter
  GradientMagnitudeImageFilter
  GradientMagnitudeRecursiveGaussianImageFilter
  GradientRecursiveGaussianImageFilter
  LaplacianRecursiveGaussianImageFilter
  RecursiveGaussianImageFilter
  SobelEdgeDetectionImageFilter
)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-DerivativeImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestDerivativeImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-GradientMagnitudeImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestGradientMagnitudeImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-GradientMagnitudeRecursiveGaussianImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestGradientMagnitudeRecursiveGaussianImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-GradientRecursiveGaussianImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestGradientRecursiveGaussianImageFilter ${DATA}/Gourds.png ${TEMP}/GradientRecursiveGaussianImageFilter.mha)

ADD_TEST(${KIT}-LaplacianRecursiveGaussianImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestLaplacianRecursiveGaussianImageFilter ${DATA}/Yinyang.png)

ADD_TEST(${KIT}-RecursiveGaussianImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestRecursiveGaussianImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-SobelEdgeDetectionImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestSobelEdgeDetectionImageFilter ${DATA}/Gourds.png)
endif()
