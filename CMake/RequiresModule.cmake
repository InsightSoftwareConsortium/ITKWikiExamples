macro(Requires_Module example module)
  if(NOT ITK${module}_LOADED)
    message(STATUS "ITKWikiExamples: ${example} requires ITK${module} but ITK${module} is not loaded")
    string(REGEX REPLACE "[^;]*${example}.cxx"
           "" ALL_FILES "${ALL_FILES}")
  endif()
endmacro()

macro(Requires_RemoteModule example module)
  if(NOT ${module}_LOADED)
    message(STATUS "ITKWikiExamples: ${example} requires the Remote module ${module} but ${module} is not loaded")
    string(REGEX REPLACE "[^;]*${example}.cxx"
           "" ALL_FILES "${ALL_FILES}")
  endif()
endmacro()
