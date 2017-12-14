project (Iterators)

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

Requires_Version(ShapedNeighborhoodIterator 4.7 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach()

if(BUILD_TESTING)
# Testing
set(KIT Iterators)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
  ConstNeighborhoodIterator
  ImageRegionConstIterator
  ImageRegionConstIteratorWithIndex
  ImageRegionIterator
  ImageRegionIteratorWithIndex
  NeighborhoodIterator
)

set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-ConstNeighborhoodIterator ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestConstNeighborhoodIterator ${DATA}/cthead1.png)

add_test(${KIT}-ImageRegionConstIterator ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestImageRegionConstIterator ${DATA}/cthead1.png)

add_test(${KIT}-ImageRegionConstIteratorWithIndex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestImageRegionConstIteratorWithIndex ${DATA}/cthead1.png)

add_test(${KIT}-ImageRegionIterator ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestImageRegionIterator ${DATA}/cthead1.png)

add_test(${KIT}-ImageRegionIteratorWithIndex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestImageRegionIteratorWithIndex ${DATA}/cthead1.png)

add_test(${KIT}-NeighborhoodIterator ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestNeighborhoodIterator ${DATA}/cthead1.png)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
