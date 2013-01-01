PROJECT (IO)

IF(NOT ITKWikiExamples_BINARY_DIR)
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ENDIF(NOT ITKWikiExamples_BINARY_DIR)

#
# Build all .cxx files in the directory
FILE(GLOB ALL_FILES *.cxx)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)
set(VERSION_MIN "4.0")
Requires_Version(TransformFileWriter 4.0 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT IO)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  ImageFileReader
  ImageToVTKImageFilter
  ReadUnknownImageType
  ReadVectorImage
  TransformFileReader
  VolumeFromSlices
)

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

ADD_TEST(${KIT}-ImageFileReader ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestImageFileReader ${DATA}/Yinyang.png)

ADD_TEST(${KIT}-ReadUnknownImageType ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestReadUnknownImageType ${DATA}/Gourds.png)

ADD_TEST(${KIT}-ReadVectorImage ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestReadVectorImage ${DATA}/GourdsVector.mha)

ADD_TEST(${KIT}-ImageToVTKImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestImageToVTKImageFilter ${DATA}/Gourds.png)

ADD_TEST(${KIT}-TransformFileReader ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestTransformFileReader ${DATA}/transform.tfm)

ADD_TEST(${KIT}-VolumeFromSlices ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestVolumeFromSlices ${DATA}/VHF%03d.png 1 10 ${TEMP}/VHF.mhd)
endif()
