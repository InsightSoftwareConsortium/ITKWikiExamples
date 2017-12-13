project (EdgesAndGradients)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
endif()

#
# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)
foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach()

if(BUILD_TESTING)
# Testing
set(KIT EdgesAndGradients)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
  DerivativeImageFilter
  GradientMagnitudeImageFilter
  GradientMagnitudeRecursiveGaussianImageFilter
  GradientRecursiveGaussianImageFilter
  LaplacianRecursiveGaussianImageFilter
  RecursiveGaussianImageFilter
  SobelEdgeDetectionImageFilter
  ZeroCrossingBasedEdgeDetectionImageFilter
)

set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-DerivativeImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestDerivativeImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-GradientMagnitudeImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGradientMagnitudeImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-GradientMagnitudeRecursiveGaussianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGradientMagnitudeRecursiveGaussianImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-GradientRecursiveGaussianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGradientRecursiveGaussianImageFilter ${DATA}/Gourds.png ${TEMP}/GradientRecursiveGaussianImageFilter.mha)

add_test(${KIT}-LaplacianRecursiveGaussianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLaplacianRecursiveGaussianImageFilter ${DATA}/Yinyang.png)

add_test(${KIT}-RecursiveGaussianImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRecursiveGaussianImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-SobelEdgeDetectionImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestSobelEdgeDetectionImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-ZeroCrossingBasedEdgeDetectionImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestZeroCrossingBasedEdgeDetectionImageFilter ${DATA}/Gourds.png 10)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
