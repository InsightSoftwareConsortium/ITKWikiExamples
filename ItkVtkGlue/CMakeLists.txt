cmake_minimum_required(VERSION 2.6)
PROJECT (ItkVtkGlue)

IF(NOT ITKWikiExamples_BINARY_DIR)
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
SET(LIBRARY_OUTPUT_PATH ${ItkVtkGlue_BINARY_DIR})
ENDIF(NOT ITKWikiExamples_BINARY_DIR)
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
