PROJECT (ItkVtkGlue)

IF(NOT ITKWikiExamples_BINARY_DIR)
FIND_PACKAGE(VTK REQUIRED)
IF(NOT VTK_USE_RENDERING)
  MESSAGE(FATAL_ERROR "Example ${PROJECT_NAME} requires VTK_USE_RENDERING.")
ENDIF(NOT VTK_USE_RENDERING)
INCLUDE(${VTK_USE_FILE})
ENDIF(NOT ITKWikiExamples_BINARY_DIR)

#
# Build all .cxx files in the directory
set(ItkVtkGlue_SRCS
  QuickView.cxx
)
add_library(ItkVtkGlue ${ItkVtkGlue_SRCS})
target_link_libraries(ItkVtkGlue ITKBasicFilters ITKCommon ITKIO ITKNumerics vtkIO vtkHybrid)
