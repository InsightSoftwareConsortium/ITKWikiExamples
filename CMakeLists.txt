cmake_minimum_required(VERSION 2.8.6 FATAL_ERROR)
 
PROJECT(ITKWikiExamples)
if (Fetch_WikiExamples OR Module_WikiExamples)
  message(STATUS "ITKWikiExamples: Building ITKWikiExamples as a Remote Module")
  message(STATUS "ITKWikiExamples: All executables will have a \"wiki\" prefix")
  set(WIKI wiki)
  message(STATUS "ITKWikiExamples: Review classes cannot be used in Remote modules")
  message(STATUS "ITKWikiExamples: Disabling wiki examples that require Review classes")
  itk_module_impl()
  set(ITK_NO_IO_FACTORY_REGISTER_MANAGER "")
  if(NOT EXISTS "${ITK_BINARY_DIR}/ITKConfig.cmake")
    return()
  endif()
  find_package(ITK PATHS ${ITK_BINARY_DIR})
  include(${ITK_USE_FILE})
  set(ITK_LIBRARIES ${ITK_LIBRARIES} ${VTK_LIBRARIES})
else()
  message(STATUS "ITKWikiExamples: Building ITKWikiExamples stand-alone")

# See if VTK is using OpenGL or OpenGL2
  find_package(VTK CONFIG)
  if (NOT VTK_RENDERING_BACKEND)
    set(VTK_RENDERING_BACKEND OpenGL)
  endif()

  find_package(VTK COMPONENTS
    vtkFiltersSources
    vtkIOImage
    vtkIOXML
    vtkInteractionImage
    vtkRenderingAnnotation
    vtkRenderingVolume${VTK_RENDERING_BACKEND}
    vtkRendering${VTK_RENDERING_BACKEND}
    vtkTestingRendering
  )
  include(${VTK_USE_FILE})
  set(VTK_COMPONENTS ${VTK_LIBRARIES})

  find_package(ITK REQUIRED)
  include(${ITK_USE_FILE})
  set(ITK_LIBRARIES ${ITK_LIBRARIES} ${VTK_COMPONENTS})
endif()

# Wiki examples use the VTK testing framework
if ( ${VTK_VERSION} VERSION_GREATER "6.0")
  if ( NOT vtkTestingRendering_LOADED)
    message(FATAL_ERROR "ITKWikiExamples: The associated VTK build must either have BUILD_TESTING:BOOL=ON or Module_vtkTestingRendering:BOOL=ON")
  endif()
endif()

if (NOT ITK_VERSION)
  set(ITK_VERSION "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}.${ITK_VERSION_PATCH}")
endif()
message(STATUS "ITKWikiExamples: ITK VERSION: ${ITK_VERSION}")
message(STATUS "ITKWikiExamples: VTK VERSION: ${VTK_VERSION}(${VTK_RENDERING_BACKEND})")

ENABLE_TESTING()
INCLUDE(CTest)
CONFIGURE_FILE(${ITKWikiExamples_SOURCE_DIR}/CMake/CTestCustom.ctest.in
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
if(NOT Fetch_WikiExamples AND NOT Module_WikiExamples)
  IF(NOT LIBRARY_OUTPUT_PATH)
    SET (LIBRARY_OUTPUT_PATH ${ITKWikiExamples_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
  ENDIF(NOT LIBRARY_OUTPUT_PATH)
  IF(NOT EXECUTABLE_OUTPUT_PATH)
    SET (EXECUTABLE_OUTPUT_PATH ${ITKWikiExamples_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
  ENDIF(NOT EXECUTABLE_OUTPUT_PATH)
  MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
else()
    SET (EXECUTABLE_OUTPUT_PATH ${ITK_BINARY_DIR}/bin)
endif()
message(STATUS "ITKWikiExamples: Example executables are stored in ${EXECUTABLE_OUTPUT_PATH}")
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
    message(FATAL_ERROR "ITKWikiExamples: Examples require building ITK with Review turned on")
  endif()
endif ()

if (NOT ITKVtkGlue_LOADED)
  message(STATUS "ITKWikiExamples: ItkVtkGlue will be built")
  set(ItkVtkGlue "ItkVtkGlue")
  INCLUDE_DIRECTORIES(ItkVtkGlue)
  ADD_SUBDIRECTORY(ItkVtkGlue)
else()
  message(STATUS "ITKWikiExamples: ItkVtkGlue is provided as part of the ITK build")
endif()
ADD_SUBDIRECTORY(Curves)
ADD_SUBDIRECTORY(DICOM)
ADD_SUBDIRECTORY(EdgesAndGradients)
ADD_SUBDIRECTORY(Functions)
ADD_SUBDIRECTORY(IO)
ADD_SUBDIRECTORY(ImageProcessing)
ADD_SUBDIRECTORY(ImageSegmentation)
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
  MARK_AS_ADVANCED( RUN_ADMIN_TESTS )
ENDIF( RUN_ADMIN_TESTS )
