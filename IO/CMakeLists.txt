PROJECT (IO)

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
SET(KIT IO)
SET(KIT_LIBS ItkVtkGlue)
SET(NEEDS_ARGS
  DisplayAnImage
  ImageFileReader
  ReadUnknownImageType
)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-DisplayAnImage ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestDisplayAnImage ${DATA}/Gourds.png)

ADD_TEST(${KIT}-ImageFileReader ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestImageFileReader ${DATA}/Yinyang.png)

ADD_TEST(${KIT}-ReadUnknownImageType ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestReadUnknownImageType ${DATA}/Gourds.png)
