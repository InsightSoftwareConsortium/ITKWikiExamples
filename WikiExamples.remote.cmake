#
# ITK WikiExamples
#  This remote module require a VTK build
#  The following CMake variable must be set for the ITK build
#  BUILD_EXAMPLES:BOOL=OFF
#  ITKV3_COMPATIBILITY:BOOL=ON
#  Module_ITKVtkGlue:BOOL=ON
# 
# Check for requires cmake variables
if (BUILD_EXAMPLES)
  message(FATAL_ERROR "WikiExamples: The ITK build must be configured with BUILD_EXAMPLES:BOOL=OFF")
endif()

if (NOT ITKV3_COMPATIBILITY)
  message(FATAL_ERROR "WikiExamples: The ITK build must be configured with ITKV3_COMPATIBILITY:BOOL=ON")
endif()

if (NOT Module_ITKVtkGlue)
  message(FATAL_ERROR "WikiExamples: The ITK build must be configured with Module_ITKVtkGlue:BOOL=ON")
endif()

itk_fetch_module(WikiExamples
  "A collection of examples that illustrate how to use ITK."
  GIT_REPOSITORY https://github.com/InsightSoftwareConsortium/ITKWikiExamples.git
  GIT_TAG master
  )
