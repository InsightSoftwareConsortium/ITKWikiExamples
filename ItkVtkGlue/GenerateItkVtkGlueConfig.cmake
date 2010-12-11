# Generate the ItkVtkGlueConfig.cmake file in the build tree.
# The file tells external projects how to use ItkVtkGlue.

# Settings specific to build trees
#
#
set(ItkVtkGlue_USE_FILE_CONFIG ${ItkVtkGlue_BINARY_DIR}/UseItkVtkGlue.cmake)
set(ItkVtkGlue_INCLUDE_DIRS_CONFIG
  ${ItkVtkGlue_BINARY_DIR}
  ${ItkVtkGlue_SOURCE_DIR}
  )
set(ItkVtkGlue_LIBRARY_DIRS_CONFIG
  ${LIBRARY_OUTPUT_PATH}
  )
set(ItkVtkGlue_BINARY_DIR_CONFIG
  ${ItkVtkGlue_BINARY_DIR}
  )

configure_file(
  ${ItkVtkGlue_SOURCE_DIR}/ItkVtkGlueConfig.cmake.in
  ${ItkVtkGlue_BINARY_DIR}/ItkVtkGlueConfig.cmake
  @ONLY IMMEDIATE
  )

configure_file(
  ${ItkVtkGlue_SOURCE_DIR}/UseItkVtkGlue.cmake.in
  ${ItkVtkGlue_USE_FILE_CONFIG}
  @ONLY IMMEDIATE
  )

