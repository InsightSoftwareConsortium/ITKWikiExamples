Project (Morphology)

IF(NOT ITKWikiExamples_BINARY_DIR)
FIND_PACKAGE(VTK REQUIRED)
IF(NOT VTK_USE_RENDERING)
  MESSAGE(FATAL_ERROR "Example ${PROJECT_NAME} requires VTK_USE_RENDERING.")
ENDIF(NOT VTK_USE_RENDERING)
INCLUDE(${VTK_USE_FILE})
ENDIF(NOT ITKWikiExamples_BINARY_DIR)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)

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
SET(KIT Morphology)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  BinaryMorphologicalClosingImageFilter
  BinaryMorphologicalOpeningImageFilter
  BinaryDilateImageFilter
  BinaryErodeImageFilter
  FlatStructuringElement
  GrayscaleDilateImageFilter
  GrayscaleErodeImageFilter
)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-BinaryMorphologicalClosingImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryMorphologicalClosingImageFilter ${DATA}/Yinyang.png 20)

ADD_TEST(${KIT}-BinaryMorphologicalOpeningImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryMorphologicalOpeningImageFilter ${DATA}/Yinyang.png 20)

ADD_TEST(${KIT}-BinaryDilateImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryDilateImageFilter ${DATA}/Yinyang.png 2)

ADD_TEST(${KIT}-BinaryErodeImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryErodeImageFilter ${DATA}/Yinyang.png 2)

ADD_TEST(${KIT}-FlatStructuringElement ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestFlatStructuringElement ${DATA}/Yinyang.png 2)

ADD_TEST(${KIT}-GrayscaleDilateImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGrayscaleDilateImageFilter ${DATA}/Gourds.png 2)

ADD_TEST(${KIT}-GrayscaleErodeImageFilter ${RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGrayscaleErodeImageFilter ${DATA}/Gourds.png 2)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
