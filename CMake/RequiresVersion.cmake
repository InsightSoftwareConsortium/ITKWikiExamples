macro(Requires_Version example version_min)
set(ITK_VERSION "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}.${ITK_VERSION_PATCH}")
  if(ITK_VERSION VERSION_LESS "${version_min}")
  message(STATUS "${example} requires ITK version ${version_min} or newer but the current version is ${ITK_VERSION}")
  string(REGEX REPLACE "[^;]*${example}.cxx"
         "" ALL_FILES "${ALL_FILES}")
  endif()
endmacro()
