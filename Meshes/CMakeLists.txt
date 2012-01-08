PROJECT (Meshes)

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
Requires_Version(QuadEdgeMeshParameterizationFilter 4.0 ALL_FILES)
Requires_Version(QuadEdgeMeshNormalFilter 4.0 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${EXAMPLE} ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${EXAMPLE} ItkVtkGlue)
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT Meshes)
SET(KIT_LIBS ItkVtkGlue)
SET(NEEDS_ARGS
  QuadEdgeMeshNormalFilter
  QuadEdgeMeshParameterizationFilter
)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-QuadEdgeMeshNormalFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestQuadEdgeMeshNormalFilter ${DATA}/sphere.vtk 1)

ADD_TEST(${KIT}-QuadEdgeMeshParameterizationFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestQuadEdgeMeshParameterizationFilter ${DATA}/disk.vtk 1 1 ${TEMP}/QuadEdgeMeshParameterizationFilter.vtk)
endif()
