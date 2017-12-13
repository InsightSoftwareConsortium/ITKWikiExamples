project (Meshes)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
endif()

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)

#
# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)
Requires_Version(QuadEdgeMeshParameterizationFilter 4.0 ALL_FILES)
Requires_Version(QuadEdgeMeshNormalFilter 4.0 ALL_FILES)
Requires_RemoteModule(LoopTriangleCellSubdivision SubdivisionQuadEdgeMeshFilter ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach()

if(BUILD_TESTING)
# Testing
set(KIT Meshes)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
  QuadEdgeMeshNormalFilter
  QuadEdgeMeshParameterizationFilter
  ReadWrite
)

set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-QuadEdgeMeshNormalFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestQuadEdgeMeshNormalFilter ${DATA}/sphere.vtk 1)

add_test(${KIT}-QuadEdgeMeshParameterizationFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestQuadEdgeMeshParameterizationFilter ${DATA}/disk.vtk 1 1 ${TEMP}/QuadEdgeMeshParameterizationFilter.vtk)

add_test(${KIT}-ReadWrite ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestreadWrite 6)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
