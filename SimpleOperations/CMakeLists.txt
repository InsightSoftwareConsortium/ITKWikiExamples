project (SimpleOperations)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
endif(NOT ITKWikiExamples_BINARY_DIR)

#
# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)
foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
set(KIT SimpleOperations)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
  RegionOverlap
  WidthHeight
)

set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-RegionOverlap ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestRegionOverlap ${TEMP}/RegionOverlap.png)

add_test(${KIT}-WidthHeight ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestWidthHeight ${DATA}/cthead1.png)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
