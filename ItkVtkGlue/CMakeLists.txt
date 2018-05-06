cmake_minimum_required(VERSION 3.10.2)
project (ItkVtkGlue)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ItkVtkGlue_BINARY_DIR})
endif()
include(GenerateItkVtkGlueConfig.cmake)

#-----------------------------------------------------------------------------
# Configure ItkVtkGlueConfig.cmake for the build tree.
configure_file(${ItkVtkGlue_SOURCE_DIR}/ItkVtkGlueConfig.cmake.in
               ${ItkVtkGlue_BINARY_DIR}/ItkVtkGlueConfig.cmake @ONLY IMMEDIATE)
# Copy the UseItkVtk.cmake file to the binary tree
configure_file(${ItkVtkGlue_SOURCE_DIR}/UseItkVtkGlue.cmake.in
               ${ItkVtkGlue_BINARY_DIR}/UseItkVtkGlue.cmake @ONLY IMMEDIATE)

#
# Build all .cxx files in the directory
set(ItkVtkGlue_SRCS
  QuickView.cxx
)

if(NOT VTK_LIBRARIES)
  set(VTK_LIBRARIES vtkIO;vtkHybrid;vtkVolumeRendering)
endif()

add_library(ItkVtkGlue ${ItkVtkGlue_SRCS})
target_link_libraries(ItkVtkGlue ${ITK_LIBRARIES} ${VTK_LIBRARIES})
