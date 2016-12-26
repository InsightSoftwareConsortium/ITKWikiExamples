#---------------------------------------------------------------------------
# Get and build itk

# April 12, 2016
set( ITK_TAG v4.9.1)

ExternalProject_Add( ITK
  GIT_REPOSITORY "${git_protocol}://itk.org/ITK.git"
  GIT_TAG "${ITK_TAG}"
  SOURCE_DIR ITK
  BINARY_DIR ITK-build
  CMAKE_GENERATOR ${gen}
  DEPENDS VTK
  CMAKE_ARGS
    ${ep_common_args}
    -DBUILD_SHARED_LIBS:BOOL=ON
    -DBUILD_EXAMPLES:BOOL=OFF
    -DBUILD_TESTING:BOOL=OFF
    -DITK_BUILD_DEFAULT_MODULES:BOOL=ON
    -DModule_ITKReview:BOOL=ON
    -DModule_ITKVtkGlue:BOOL=ON
    -DVTK_DIR:PATH=${VTK_DIR}
  INSTALL_COMMAND ""
)

set(ITK_DIR ${CMAKE_BINARY_DIR}/ITK-build)
