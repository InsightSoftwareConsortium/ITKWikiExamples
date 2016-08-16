PROJECT (ImageSegmentation)

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

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT ImageSegmentation)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  ExtractContourWithSnakes
  ConfidenceConnectedImageFilter
  ExtractLargestConnectedComponentFromBinaryImage
  LabelContourImageFilter
)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-ExtractContourWithSnakes ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests

  TestExtractContourWithSnakes 100 0.01 0.4 0.07 4 6000)

ADD_TEST(${KIT}-ConfidenceConnectedImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestConfidenceConnectedImageFilter ${DATA}/cthead1.png 127 127)

ADD_TEST(${KIT}-ExtractLargestConnectedComponentFromBinaryImage ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestExtractLargestConnectedComponentFromBinaryImage ${DATA}/BinaryImageWithVariousShapes01.png)

ADD_TEST(${KIT}-LabelContourImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLabelContourImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
