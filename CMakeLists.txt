cmake_minimum_required(VERSION 2.6)
 
PROJECT(ITKWikiExamples)
 
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

if (NOT ITK_VERSION)
set(ITK_VERSION "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}.${ITK_VERSION_PATCH}")
endif()
message(STATUS "ITK VERSION: ${ITK_VERSION}")
message(STATUS "VTK VERSION: ${VTK_VERSION}")

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

if(WIN32)
  # Some libraries (e.g. vxl libs) have no dllexport markup, so we can
  # build full shared libraries only with the GNU toolchain.
  if(CMAKE_COMPILER_IS_GNUCXX)
    # CMake adds --enable-all-exports on Cygwin (since Cygwin is
    # supposed to be UNIX-like), but we need to add it explicitly for
    # a native windows build with the MinGW tools.
    if(MINGW)
      set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS
        "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import")
      set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS
        "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import")
      set(CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-auto-import")
    endif(MINGW)
  else(CMAKE_COMPILER_IS_GNUCXX)
   if(BUILD_SHARED_LIBS)
     set(ITK_LIBRARY_BUILD_TYPE "SHARED")
   else(BUILD_SHARED_LIBS)
     set(ITK_LIBRARY_BUILD_TYPE "STATIC")
   endif(BUILD_SHARED_LIBS)
   set(BUILD_SHARED_LIBS OFF)
  endif(CMAKE_COMPILER_IS_GNUCXX)
endif(WIN32)

if (ITK_VERSION_MAJOR STRLESS 4)
  if(NOT ITK_USE_REVIEW)
    message(FATAL_ERROR "Examples require building ITK with Review turned on")
  endif()
endif ()

INCLUDE_DIRECTORIES(ItkVtkGlue)
ADD_SUBDIRECTORY(ItkVtkGlue)
ADD_SUBDIRECTORY(Curves)
ADD_SUBDIRECTORY(DICOM)
ADD_SUBDIRECTORY(EdgesAndGradients)
ADD_SUBDIRECTORY(Functions)
ADD_SUBDIRECTORY(IO)
ADD_SUBDIRECTORY(ImageProcessing)
ADD_SUBDIRECTORY(Images)
ADD_SUBDIRECTORY(Inspection)
ADD_SUBDIRECTORY(Iterators)
ADD_SUBDIRECTORY(Math)
ADD_SUBDIRECTORY(Meshes)
ADD_SUBDIRECTORY(Metrics)
ADD_SUBDIRECTORY(Morphology)
ADD_SUBDIRECTORY(Operators)
ADD_SUBDIRECTORY(PointSet)
ADD_SUBDIRECTORY(Registration)
ADD_SUBDIRECTORY(Segmentation)
ADD_SUBDIRECTORY(SimpleOperations)
ADD_SUBDIRECTORY(Smoothing)
ADD_SUBDIRECTORY(SpatialObjects)
ADD_SUBDIRECTORY(SpectralAnalysis)
ADD_SUBDIRECTORY(Statistics)
ADD_SUBDIRECTORY(Utilities)
ADD_SUBDIRECTORY(VectorImages)

OPTION(RUN_ADMIN_TESTS "Run the admin tests" OFF)
IF( RUN_ADMIN_TESTS )
  ADD_TEST(CheckPagesForImages ${ITKWikiExamples_SOURCE_DIR}/Admin/CheckPagesForImages)
  SET_TESTS_PROPERTIES(CheckPagesForImages PROPERTIES TIMEOUT 2000)
ENDIF( RUN_ADMIN_TESTS )
