Project (Morphology)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(VTK REQUIRED)
if(NOT VTK_USE_RENDERING)
  message(FATAL_ERROR "Example ${PROJECT_NAME} requires VTK_USE_RENDERING.")
endif()
include(${VTK_USE_FILE})
endif()

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)

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
set(KIT Morphology)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
  BinaryMorphologicalClosingImageFilter
  BinaryMorphologicalOpeningImageFilter
  BinaryDilateImageFilter
  BinaryErodeImageFilter
  FlatStructuringElement
  GrayscaleDilateImageFilter
  GrayscaleErodeImageFilter
)

set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-BinaryMorphologicalClosingImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryMorphologicalClosingImageFilter ${DATA}/Yinyang.png 20)

add_test(${KIT}-BinaryMorphologicalOpeningImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryMorphologicalOpeningImageFilter ${DATA}/Yinyang.png 20)

add_test(${KIT}-BinaryDilateImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryDilateImageFilter ${DATA}/Yinyang.png 2)

add_test(${KIT}-BinaryErodeImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestBinaryErodeImageFilter ${DATA}/Yinyang.png 2)

add_test(${KIT}-FlatStructuringElement ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestFlatStructuringElement ${DATA}/Yinyang.png 2)

add_test(${KIT}-GrayscaleDilateImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGrayscaleDilateImageFilter ${DATA}/Gourds.png 2)

add_test(${KIT}-GrayscaleErodeImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestGrayscaleErodeImageFilter ${DATA}/Gourds.png 2)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
