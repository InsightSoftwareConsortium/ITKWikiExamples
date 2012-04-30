#
# Create tests for examples
#
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
SET(MyTests "")
foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  SET(MyTests ${MyTests} Test${EXAMPLE}.cxx)
  FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Test${EXAMPLE}.cxx
    "#define main Test${EXAMPLE}\n#include \"${EXAMPLE}.cxx\"\n")
  LIST(FIND NEEDS_ARGS ${EXAMPLE} SKIP_ADD)
  if(SKIP_ADD EQUAL -1)
    ADD_TEST(${KIT}-${EXAMPLE} ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests Test${EXAMPLE})
  endif(SKIP_ADD EQUAL -1)
endforeach(SOURCE_FILE)

if(${VTK_MAJOR_VERSION} GREATER 4 )
  ## Version is OK if greater than 5.5
else()
  MESSAGE(FATAL_ERROR "Need VTK version 5.5 or greater to include the necessary vtkTestingObjectFactory.cmake")
endif()


SET(VTK_BINARY_DIR ${ITKWikiExamples_BINARY_DIR})
SET(VTK_DATA_ROOT ${ITKWikiExamples_SOURCE_DIR}/Testing)
INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/vtkTestingObjectFactory.cmake)
ADD_EXECUTABLE(${KIT}CxxTests ${KIT}CxxTests.cxx
               ${MyTests})
TARGET_LINK_LIBRARIES(${KIT}CxxTests ${KIT_LIBS})
