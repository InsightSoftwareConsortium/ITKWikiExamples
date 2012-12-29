PROJECT (SpatialObjects)

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
  ADD_EXECUTABLE(${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${EXAMPLE} ItkVtkGlue)
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT SpatialObjects)
SET(KIT_LIBS ItkVtkGlue)
SET(NEEDS_ARGS
  EllipseSpatialObject
  SpatialObjectToImageFilter
)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-EllipseSpatialObject ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestEllipseSpatialObject ${TEMP}/EllipseSpatialObject.png)

ADD_TEST(${KIT}-SpatialObjectToImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestSpatialObjectToImageFilter ${TEMP}/SpatialObjectToImageFilter.png)
endif()
