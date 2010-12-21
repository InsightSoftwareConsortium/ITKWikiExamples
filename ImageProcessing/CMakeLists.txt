PROJECT (ImageProcessing)

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
  TARGET_LINK_LIBRARIES(${EXAMPLE} ITKReview ItkVtkGlue)
endforeach(SOURCE_FILE)

# Testing
SET(KIT ImageProcessing)
SET(KIT_LIBS ITKReview ItkVtkGlue)
SET(NEEDS_ARGS
  BinaryThresholdImageFilter
  MinimumMaximumImageCalculator
  NormalizeImageFilter
)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-BinaryThresholdImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestBinaryThresholdImageFilter ${DATA}/cthead1.png 100 255)

ADD_TEST(${KIT}-NormalizeImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestNormalizeImageFilter ${DATA}/cthead1.png)

ADD_TEST(${KIT}-MinimumMaximumImageCalculator ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestMinimumMaximumImageCalculator ${DATA}/cthead1.png)
