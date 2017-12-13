cmake_minimum_required(VERSION 2.8)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

project(ITKWikiExamples)
include( ${CMAKE_SOURCE_DIR}/Superbuild.cmake )
