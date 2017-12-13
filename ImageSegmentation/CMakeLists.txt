project (ImageSegmentation)

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

foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach()

if(BUILD_TESTING)
# Testing
set(KIT ImageSegmentation)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
  ExtractContourWithSnakes
  ConfidenceConnectedImageFilter
  ExtractLargestConnectedComponentFromBinaryImage
  LabelContourImageFilter
)

set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-ExtractContourWithSnakes ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests

  TestExtractContourWithSnakes 100 0.01 0.4 0.07 4 6000)

add_test(${KIT}-ConfidenceConnectedImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestConfidenceConnectedImageFilter ${DATA}/cthead1.png 127 127)

add_test(${KIT}-ExtractLargestConnectedComponentFromBinaryImage ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestExtractLargestConnectedComponentFromBinaryImage ${DATA}/BinaryImageWithVariousShapes01.png)

add_test(${KIT}-LabelContourImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestLabelContourImageFilter ${DATA}/BinaryImageWithVariousShapes01.png)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
