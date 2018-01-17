cmake_minimum_required(VERSION 3.9.5)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

project(ITKWikiExamples)
include( ${CMAKE_SOURCE_DIR}/Superbuild.cmake )
