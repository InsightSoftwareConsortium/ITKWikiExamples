PROJECT (PointSet)

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
SET(KIT PointSet)
SET(KIT_LIBS ItkVtkGlue)
SET(NEEDS_ARGS
  ReadPointSet
  WritePointSet
)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-ReadPointSet ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestReadPointSet ${DATA}/sphere.vtk)

ADD_TEST(${KIT}-WritePointSet ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestWritePointSet ${TEMP}/WritePointSet.vtk)
