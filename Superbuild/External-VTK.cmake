#-----------------------------------------------------------------------------
# Get and build VTK
#
#-----------------------------------------------------------------------------
# August 2015
set( VTK_TAG v6.3.0 )

ExternalProject_Add(VTK
  GIT_REPOSITORY "${git_protocol}://vtk.org/VTK.git"
  GIT_TAG "${VTK_TAG}"
  SOURCE_DIR VTK
  BINARY_DIR VTK-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DVTK_RENDERING_BACKEND:STRING=OpenGL
    -DBUILD_SHARED_LIBS:BOOL=TRUE
    -DVTK_DEBUG_LEAKS:BOOL=TRUE
    -DBUILD_EXAMPLES:BOOL=OFF
    -DBUILD_TESTING:BOOL=OFF
    -DVTK_BUILD_ALL_MODULES_FOR_TESTS:BOOL=OFF
    -DVTK_Group_Rendering:BOOL=OFF
    -DVTK_Group_StandAlone:BOOL=OFF
    -DModule_vtkFiltersSources:BOOL=ON
    -DModule_vtkIOImage:BOOL=ON
    -DModule_vtkIOXML:BOOL=ON
    -DModule_vtkInteractionImage:BOOL=ON
    -DModule_vtkRenderingAnnotation:BOOL=ON
    -DModule_vtkRenderingVolumeOpenGL:BOOL=ON
    -DModule_vtkTestingRendering:BOOL=ON

  INSTALL_COMMAND ""
)

set(VTK_DIR ${CMAKE_BINARY_DIR}/VTK-build)
