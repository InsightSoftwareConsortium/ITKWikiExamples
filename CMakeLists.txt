cmake_minimum_required(VERSION 2.6)
 
PROJECT(ITKWikiExamples)
 
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

ENABLE_TESTING()
INCLUDE(CTest)
CONFIGURE_FILE(${ITKWikiExamples_SOURCE_DIR}/CMake//CTestCustom.ctest.in
  ${ITKWikiExamples_BINARY_DIR}/CMake/CTestCustom.ctest @ONLY)
FILE(WRITE ${ITKWikiExamples_BINARY_DIR}/CTestCustom.cmake
  "INCLUDE(\"${ITKWikiExamples_BINARY_DIR}/CMake/CTestCustom.ctest\")\n")
CONFIGURE_FILE(
  ${ITKWikiExamples_SOURCE_DIR}/CMake/SampleBuildTest.cmake.in
  ${ITKWikiExamples_BINARY_DIR}/SampleBuildTest.cmake @ONLY)

#-----------------------------------------------------------------------------
# Disable deprecation warnings for standard C and STL functions in VS2005
# and later
IF(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS) 
ENDIF(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400) 

#-----------------------------------------------------------------------------
# Output directories.
IF(NOT LIBRARY_OUTPUT_PATH)
  SET (LIBRARY_OUTPUT_PATH ${ITKWikiExamples_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
ENDIF(NOT LIBRARY_OUTPUT_PATH)
IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET (EXECUTABLE_OUTPUT_PATH ${ITKWikiExamples_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

INCLUDE_DIRECTORIES(ItkVtkGlue)
ADD_SUBDIRECTORY(ItkVtkGlue)
ADD_SUBDIRECTORY(Images)

OPTION(RUN_ADMIN_TESTS "Run the admin tests" OFF)
IF( RUN_ADMIN_TESTS )
  ADD_TEST(CheckPagesForImages ${ITKWikiExamples_SOURCE_DIR}/Admin/CheckPagesForImages)
  SET_TESTS_PROPERTIES(CheckPagesForImages PROPERTIES TIMEOUT 2000)
ENDIF( RUN_ADMIN_TESTS )
