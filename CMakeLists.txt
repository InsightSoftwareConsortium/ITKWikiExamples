cmake_minimum_required(VERSION 3.9.5 FATAL_ERROR)
 
project(ITKWikiExamples)
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

enable_testing()
include(CTest)
configure_file(${ITKWikiExamples_SOURCE_DIR}/CMake/CTestCustom.ctest.in
  ${ITKWikiExamples_BINARY_DIR}/CMake/CTestCustom.ctest @ONLY)
file(WRITE ${ITKWikiExamples_BINARY_DIR}/CTestCustom.cmake
  "include(\"${ITKWikiExamples_BINARY_DIR}/CMake/CTestCustom.ctest\")\n")
configure_file(
  ${ITKWikiExamples_SOURCE_DIR}/CMake/SampleBuildTest.cmake.in
  ${ITKWikiExamples_BINARY_DIR}/SampleBuildTest.cmake @ONLY)

#-----------------------------------------------------------------------------
# Disable deprecation warnings for standard C and STL functions in VS2005
# and later
if(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_SCL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS) 
endif() 

#-----------------------------------------------------------------------------
# Output directories.
if(NOT Fetch_WikiExamples AND NOT Module_WikiExamples)
  if(NOT LIBRARY_OUTPUT_PATH)
    set (LIBRARY_OUTPUT_PATH ${ITKWikiExamples_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
  endif()
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ITKWikiExamples_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
  endif()
  mark_as_advanced(LIBRARY_OUTPUT_PATH CMAKE_RUNTIME_OUTPUT_DIRECTORY)
else()
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ITK_BINARY_DIR}/bin)
endif()
message(STATUS "ITKWikiExamples: Example executables are stored in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
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
    endif()
  else()
   if(BUILD_SHARED_LIBS)
     set(ITK_LIBRARY_BUILD_TYPE "SHARED")
   else()
     set(ITK_LIBRARY_BUILD_TYPE "STATIC")
   endif()
   set(BUILD_SHARED_LIBS OFF)
  endif()
endif()

if (ITK_VERSION_MAJOR STRLESS 4)
  if(NOT ITK_USE_REVIEW)
    message(FATAL_ERROR "ITKWikiExamples: Examples require building ITK with Review turned on")
  endif()
endif ()

if (NOT ITKVtkGlue_LOADED)
  message(STATUS "ITKWikiExamples: ItkVtkGlue will be built")
  set(ItkVtkGlue "ItkVtkGlue")
  include_directories(ItkVtkGlue)
  add_subdirectory(ItkVtkGlue)
else()
  message(STATUS "ITKWikiExamples: ItkVtkGlue is provided as part of the ITK build")
endif()
add_subdirectory(Curves)
add_subdirectory(DICOM)
add_subdirectory(EdgesAndGradients)
add_subdirectory(Functions)
add_subdirectory(IO)
add_subdirectory(ImageProcessing)
add_subdirectory(ImageSegmentation)
add_subdirectory(Images)
add_subdirectory(Inspection)
add_subdirectory(Iterators)
add_subdirectory(Math)
add_subdirectory(Meshes)
add_subdirectory(Metrics)
add_subdirectory(Morphology)
add_subdirectory(Operators)
add_subdirectory(PointSet)
add_subdirectory(Registration)
add_subdirectory(Segmentation)
add_subdirectory(SimpleOperations)
add_subdirectory(Smoothing)
add_subdirectory(SpatialObjects)
add_subdirectory(SpectralAnalysis)
add_subdirectory(Statistics)
add_subdirectory(Utilities)
add_subdirectory(VectorImages)

option(RUN_ADMIN_TESTS "Run the admin tests" OFF)
if( RUN_ADMIN_TESTS )
  add_test(CheckPagesForImages ${ITKWikiExamples_SOURCE_DIR}/Admin/CheckPagesForImages)
  set_tests_properties(CheckPagesForImages PROPERTIES TIMEOUT 2000)
  mark_as_advanced( RUN_ADMIN_TESTS )
endif()
