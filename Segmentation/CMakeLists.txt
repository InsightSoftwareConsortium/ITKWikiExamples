PROJECT (Segmentation)

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

Requires_Version(LabelMapContourOverlayImageFilter 4.0 ALL_FILES)
Requires_Module(ContourExtractor2DImageFilter Review ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT Segmentation)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  ConnectedComponentImageFilterDemo
  ThresholdImageFilterDemo
  OtsuMultipleThresholdsImageFilter
  OtsuThresholdImageFilter
  WatershedImageFilter
)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-ConnectedComponentImageFilterDemo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestConnectedComponentImageFilterDemo ${DATA}/cthead1.png)

ADD_TEST(${KIT}-ThresholdImageFilterDemo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestThresholdImageFilterDemo ${DATA}/cthead1.png)

ADD_TEST(${KIT}-OtsuMultipleThresholdsImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestOtsuMultipleThresholdsImageFilter ${DATA}/cthead1.png 2)

ADD_TEST(${KIT}-OtsuThresholdImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestOtsuThresholdImageFilter ${DATA}/cthead1.png)

ADD_TEST(${KIT}-WatershedImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestWatershedImageFilter .005 .5)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
