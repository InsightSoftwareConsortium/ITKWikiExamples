project (Smoothing)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
endif(NOT ITKWikiExamples_BINARY_DIR)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)

#
# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)

Requires_Version(RGBBilateralImageFilter 4.0 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
set(KIT Smoothing)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
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

set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-BinaryMinMaxCurvatureFlowImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryMinMaxCurvatureFlowImageFilter ${DATA}/Yinyang.png 5)

add_test(${KIT}-BilateralImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBilateralImageFilter ${DATA}/Gourds.png 4 10)

add_test(${KIT}-BinomialBlurImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinomialBlurImageFilter ${DATA}/Gourds.png 5)

add_test(${KIT}-CurvatureFlowImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestCurvatureFlowImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-RGBCurvatureFlowImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBCurvatureFlowImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-DiscreteGaussianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestDiscreteGaussianImageFilter ${DATA}/Gourds.png 9)

add_test(${KIT}-MinMaxCurvatureFlowImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMinMaxCurvatureFlowImageFilter ${DATA}/Gourds.png 10)

add_test(${KIT}-RGBMinMaxCurvatureFlowImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBMinMaxCurvatureFlowImageFilter ${DATA}/Gourds.png 10)

add_test(${KIT}-MeanImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMeanImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-MedianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestMedianImageFilter ${DATA}/Gourds.png 3)

add_test(${KIT}-RGBMedianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRGBMedianImageFilter ${DATA}/Gourds.png 2)

add_test(${KIT}-SmoothingRecursiveGaussianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSmoothingRecursiveGaussianImageFilter ${DATA}/Gourds.png 3)

add_test(${KIT}-VectorGradientAnisotropicDiffusionImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVectorGradientAnisotropicDiffusionImageFilter ${DATA}/Gourds.png 10 1.5)

add_test(${KIT}-VectorCurvatureAnisotropicDiffusionImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVectorCurvatureAnisotropicDiffusionImageFilter ${DATA}/Gourds.png 10 1.5)

add_test(${KIT}-VectorMeanImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVectorMeanImageFilter ${DATA}/Gourds.png 4)

add_test(${KIT}-VectorMedianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVectorMedianImageFilter ${DATA}/Gourds.png 2)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
