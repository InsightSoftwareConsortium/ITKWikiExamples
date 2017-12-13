project (SpectralAnalysis)

if(NOT ITKWikiExamples_BINARY_DIR)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
endif(NOT ITKWikiExamples_BINARY_DIR)

include(${ITKWikiExamples_SOURCE_DIR}/CMake/RequiresVersion.cmake)

#
# Build all .cxx files in the directory
file(GLOB ALL_FILES *.cxx)
Requires_Version(CrossCorrelationInFourierDomain 4.0 ALL_FILES)
Requires_Version(RealAndImaginaryToComplexImageFilter 4.0 ALL_FILES)
Requires_Version(VnlFFTRealToComplexConjugateImageFilter 4.0 ALL_FILES)

foreach(SOURCE_FILE ${ALL_FILES})
  string(REPLACE ".cxx" "" TMP ${SOURCE_FILE})
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" EXAMPLE ${TMP})
  add_executable(${WIKI}${EXAMPLE} MACOSX_BUNDLE ${EXAMPLE}.cxx)
  target_link_libraries(${WIKI}${EXAMPLE} ${ItkVtkGlue} ${ITK_LIBRARIES})
endforeach(SOURCE_FILE)

if(BUILD_TESTING)
# Testing
set(KIT SpectralAnalysis)
set(KIT_LIBS ${ItkVtkGlue} ${ITK_LIBRARIES})
set(NEEDS_ARGS
  VnlFFTRealToComplexConjugateImageFilter
  CrossCorrelationInFourierDomain
)

set(DATA ${ITKWikiExamples_SOURCE_DIR}/Testing/Data)
set(TEMP ${ITKWikiExamples_BINARY_DIR}/Testing/Temporary)

if(NOT ITK_VERSION VERSION_LESS "4.0")
add_test(${KIT}-VnlFFTRealToComplexConjugateImageFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestVnlFFTRealToComplexConjugateImageFilter ${DATA}/cthead1.png)

add_test(${KIT}-CrossCorrelationInFourierDomain ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${KIT}CxxTests
  TestCrossCorrelationInFourierDomain ${DATA}/cthead1.png ${DATA}/cthead1.png)
endif()

include(${ITKWikiExamples_SOURCE_DIR}/CMake/ExamplesTesting.cmake)

endif()
