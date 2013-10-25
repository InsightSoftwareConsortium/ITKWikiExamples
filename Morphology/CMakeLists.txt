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

Requires_Version(FlatStructuringElementForegroundHasAccurateArea 4.6 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
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
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-BinaryMorphologicalClosingImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestBinaryMorphologicalClosingImageFilter ${DATA}/Yinyang.png 20)

ADD_TEST(${KIT}-BinaryMorphologicalOpeningImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestBinaryMorphologicalOpeningImageFilter ${DATA}/Yinyang.png 20)

ADD_TEST(${KIT}-BinaryDilateImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestBinaryDilateImageFilter ${DATA}/Yinyang.png 2)

ADD_TEST(${KIT}-BinaryErodeImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestBinaryErodeImageFilter ${DATA}/Yinyang.png 2)

ADD_TEST(${KIT}-FlatStructuringElement ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestFlatStructuringElement ${DATA}/Yinyang.png 2)

ADD_TEST(${KIT}-GrayscaleDilateImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestGrayscaleDilateImageFilter ${DATA}/Gourds.png 2)

ADD_TEST(${KIT}-GrayscaleErodeImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestGrayscaleErodeImageFilter ${DATA}/Gourds.png 2)
endif()
