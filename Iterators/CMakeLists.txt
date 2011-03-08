PROJECT (Iterators)

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
SET(KIT Iterators)
SET(KIT_LIBS ItkVtkGlue)
SET(NEEDS_ARGS
  ConstNeighborhoodIterator
  ImageRegionConstIterator
  ImageRegionConstIteratorWithIndex
  ImageRegionIterator
  ImageRegionIteratorWithIndex
  NeighborhoodIterator
)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-ConstNeighborhoodIterator ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestConstNeighborhoodIterator ${DATA}/cthead1.png)

ADD_TEST(${KIT}-ImageRegionConstIterator ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestImageRegionConstIterator ${DATA}/cthead1.png)

ADD_TEST(${KIT}-ImageRegionConstIteratorWithIndex ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestImageRegionConstIteratorWithIndex ${DATA}/cthead1.png)

ADD_TEST(${KIT}-ImageRegionIterator ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestImageRegionIterator ${DATA}/cthead1.png)

ADD_TEST(${KIT}-ImageRegionIteratorWithIndex ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestImageRegionIteratorWithIndex ${DATA}/cthead1.png)

ADD_TEST(${KIT}-NeighborhoodIterator ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestNeighborhoodIterator ${DATA}/cthead1.png)
