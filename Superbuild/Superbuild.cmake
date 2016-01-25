find_package(Git)
if(NOT GIT_FOUND)
  message(ERROR "Cannot find git. git is required for Superbuild")
endif()

option( USE_GIT_PROTOCOL "If behind a firewall turn this off to use http instead." ON)

set(git_protocol "git")

include( ExternalProject )

# Compute -G arg for configuring external projects with the same CMake gener    ator:
if(CMAKE_EXTRA_GENERATOR)
  set(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
else()
  set(gen "${CMAKE_GENERATOR}" )
endif()

set( ITKWikiExamples_DEPENDENCIES )

set(ep_common_args
    "-DCMAKE_BUILD_TYPE:STRING=Release"
)

include( ${CMAKE_SOURCE_DIR}/External-VTK.cmake )
list( APPEND ITKWikiExamples_DEPENDENCIES VTK )

include( ${CMAKE_SOURCE_DIR}/External-ITK.cmake )
list( APPEND ITKWikiExamples_DEPENDENCIES ITK )

ExternalProject_Add( ITKWikiExamples
  DEPENDS ${ITKWikiExamples_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
  BINARY_DIR ITKWikiExamples-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DBUILD_SHARED_LIBS:BOOL=FALSE
    -DBUILD_TESTING:BOOL=FALSE
     # ITK
    -DITK_DIR:PATH=${ITK_DIR}
    # VTK
    -DVTK_DIR:PATH=${VTK_DIR}
  INSTALL_COMMAND ""
)
