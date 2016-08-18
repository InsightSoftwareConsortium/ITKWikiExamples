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
Requires_RemoteModule(LoopTriangleCellSubdivision SubdivisionQuadEdgeMeshFilter ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT Meshes)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  QuadEdgeMeshNormalFilter
  QuadEdgeMeshParameterizationFilter
  ReadWrite
)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-QuadEdgeMeshNormalFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestQuadEdgeMeshNormalFilter ${DATA}/sphere.vtk 1)

ADD_TEST(${KIT}-QuadEdgeMeshParameterizationFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestQuadEdgeMeshParameterizationFilter ${DATA}/disk.vtk 1 1 ${TEMP}/QuadEdgeMeshParameterizationFilter.vtk)

ADD_TEST(${KIT}-ReadWrite ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestreadWrite 6)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
