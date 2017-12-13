#
# Create tests for examples
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(MyTests "")
foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  set(MyTests ${MyTests} Test${EXAMPLE}.cxx)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Test${EXAMPLE}.cxx
    "#define main Test${EXAMPLE}\n#include \"${EXAMPLE}.cxx\"\n")
  list(FIND NEEDS_ARGS ${EXAMPLE} SKIP_ADD)
  if(SKIP_ADD EQUAL -1)
    add_test(${KIT}-${EXAMPLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests Test${EXAMPLE})
  endif()
  set_property(TEST ${KIT}-${EXAMPLE} PROPERTY LABELS WikiExamples)
endforeach()

if(${VTK_MAJOR_VERSION} GREATER 4 )
  ## Version is OK if greater than 5.5
else()
  message(FATAL_ERROR "Need VTK version 5.5 or greater to include the necessary vtkTestingObjectFactory.cmake")
endif()


set(VTK_BINARY_DIR ${ITKWikiExamples_BINARY_DIR})
set(VTK_DATA_ROOT ${ITKWikiExamples_SOURCE_DIR}/Testing)
include(${ITKWikiExamples_SOURCE_DIR}/CMake/vtkTestingObjectFactory.cmake)
add_executable(${KIT}CxxTests ${KIT}CxxTests.cxx
               ${MyTests})
target_link_libraries(${KIT}CxxTests ${KIT_LIBS})
