project (IO)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
endif()

#
# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)
set(VERSION_MIN "4.7")
Requires_Version(TransformFileWriter 4.7 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach()

if(BUILD_TESTING)
# Testing
set(KIT IO)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
  ImageFileReader
  ImageToVTKImageFilter
  ReadUnknownImageType
  ReadVectorImage
  TransformFileReader
  VolumeFromSlices
)


set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

add_test(${KIT}-ImageFileReader ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestImageFileReader ${DATA}/Yinyang.png)

add_test(${KIT}-ReadUnknownImageType ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestReadUnknownImageType ${DATA}/Gourds.png)

add_test(${KIT}-ReadVectorImage ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestReadVectorImage ${DATA}/GourdsVector.mha)

add_test(${KIT}-ImageToVTKImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestImageToVTKImageFilter ${DATA}/Gourds.png)

add_test(${KIT}-TransformFileReader ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestTransformFileReader ${DATA}/transform.tfm)

add_test(${KIT}-VolumeFromSlices ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVolumeFromSlices ${DATA}/VHF%03d.png 1 10 ${TEMP}/VHF.mhd)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
