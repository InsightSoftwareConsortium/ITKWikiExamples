#
# Find the packages required by this module
#

# Look for VTK
message(STATUS "ITKWikiExamples: Finding VTK with required modules...")        
set(VTK_USE_FILE_INCLUDED 0)
find_package(VTK COMPONENTS
  vtkCommonCore
  vtkRenderingCore
  vtkRendering${VTK_RENDERING_BACKEND}
  vtkRenderingFreeType
  vtkInteractionStyle
  vtkIOImage
  vtkImagingSources
  vtkTestingRendering
  vtkInteractionImage
  vtkIOXML
  REQUIRED)
include(${VTK_USE_FILE})
message(STATUS "ITKWikiExamples: VTK Found.")        
