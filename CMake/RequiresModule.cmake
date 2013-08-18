macro(Requires_Module example module)
  if(NOT ITK${module}_LOADED AND NOT Module_ITK$(module))
    message(STATUS "ITKWikiExamples: ${example} requires ITK${module} but ITK${module} is not loaded")
    string(REGEX REPLACE "[^;]*${example}.cxx"
           "" ALL_FILES "${ALL_FILES}")
  endif()
endmacro()

