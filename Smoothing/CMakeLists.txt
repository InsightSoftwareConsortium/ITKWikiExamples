PROJECT (Smoothing)

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

# Testing
SET(KIT Smoothing)
SET(KIT_LIBS ItkVtkGlue)
SET(NEEDS_ARGS
  BilateralImageFilter
  BinomialBlurImageFilter
  DiscreteGaussianImageFilter
)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-BilateralImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestBilateralImageFilter ${DATA}/Gourds.png 4 10)

ADD_TEST(${KIT}-BinomialBlurImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestBinomialBlurImageFilter ${DATA}/Gourds.png 5)

ADD_TEST(${KIT}-DiscreteGaussianImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestDiscreteGaussianImageFilter ${DATA}/Gourds.png 9)
