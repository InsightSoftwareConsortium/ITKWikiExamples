PROJECT (SpectralAnalysis)

IF(NOT ITKWikiExamples_BINARY_DIR)
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ENDIF(NOT ITKWikiExamples_BINARY_DIR)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)

#
# Build all .cxx files in the directory
FILE(GLOB ALL_FILES *.cxx)
Requires_Version(CrossCorrelationInFourierDomain 4.0 ALL_FILES)
Requires_Version(RealAndImaginaryToComplexImageFilter 4.0 ALL_FILES)
Requires_Version(VnlFFTRealToComplexConjugateImageFilter 4.0 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  STRING(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  ADD_EXECUTABLE(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  TARGET_LINK_LIBRARIES(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
SET(KIT SpectralAnalysis)
SET(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
SET(NEEDS_ARGS
  VnlFFTRealToComplexConjugateImageFilter
  CrossCorrelationInFourierDomain
)

SET(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
SET(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

if(NOT ITK_VERSION VERSION_LESS "4.0")
ADD_TEST(${KIT}-VnlFFTRealToComplexConjugateImageFilter ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestVnlFFTRealToComplexConjugateImageFilter ${DATA}/cthead1.png)

ADD_TEST(${KIT}-CrossCorrelationInFourierDomain ${EXECUTABLE_OUTPUT_PATH}/${KIT}CxxTests
  TestCrossCorrelationInFourierDomain ${DATA}/cthead1.png ${DATA}/cthead1.png)
endif()

INCLUDE(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
