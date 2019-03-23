ITKWikiExamples
===============

.. image:: https://dev.azure.com/InsightSoftwareConsortium/ITKModules/_apis/build/status/InsightSoftwareConsortium.ITKWikiExamples?branchName=master
    :target: https://dev.azure.com/InsightSoftwareConsortium/ITKModules/_build/latest?definitionId=8&branchName=master
    :alt:    Build Status

Overview
--------

The ITK WikiExamples can be built in several ways:

1) A single example - go to the example page and follow instrutions
2) All examples stand-alone go to
   http://www.vtk.org/Wiki/ITK/Examples/Instructions/ForUsers#Build_all_of_the_examples
3) All examples with a superbuild that builds a proper ITK and VTK - go to
   http://www.vtk.org/Wiki/ITK/Examples/Instructions/ForUsers#Use_the_Superbuild_to_build_a_proper_VTK_and_ITK
4) As an ITKv4 Remote Module proceed as follows.

* Enable fetching
  1) Go to: https://github.com/InsightSoftwareConsortium/ITKWikiExamples/raw/master/WikiExamples.remote.cmake
  2) Right click on the "Raw" button and select "Save Link As..."
  3) Save the file in: YOUR_ITK_SOURCE_DIR/Modules/Remote

A) If you use an interactive cmake (ccmake, cmake-gui or cmakesetup)
--------------------------------------------------------------------
* Configure
  1) Go to your itk build directory and run cmake
  2) Configure

* NOTE: ITK 4.4 and earlier
  Fetch ITK Wiki Examples
  1) Open the highlighted (red) cmake entry
  2) Check the Fetch_WikiExamples box
  3) Configure

* Enable ITK Wiki Exampes
  1) Configure (yes, do it again)
  2) Open the highlighted (red) cmake entry
  3) Check the Module_WikiExamples box
  4) Configure
  5) Generate
  6) Exit cmake

B) If you use cmake from the command line
--------------------------------------
From your itk build directory
* Configure
  1) NOTE: ITK 4.4 and earlier
     cmake -DFetch_WikiExamples=ON YOUR_ITK_SOURCE_DIR
  2) cmake -DModule_WikiExamples=ON YOUR_ITK_SOURCE_DIR

Once you have configured using A) or B)
---------------------------------------
* Build WikiExamples
  From your itk build directory
  1) make

* Test WikiExamples if testing is ON
  1) cd your itk build directory
  2) cd Modules/Remote/WikiExamples
  3) ctest
