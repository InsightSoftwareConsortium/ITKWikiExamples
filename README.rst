ITKWikiExamples
===============

.. image:: https://dev.azure.com/InsightSoftwareConsortium/ITKModules/_apis/build/status/InsightSoftwareConsortium.ITKWikiExamples?branchName=master
    :target: https://dev.azure.com/InsightSoftwareConsortium/ITKModules/_build/latest?definitionId=18&branchName=master
    :alt:    Build Status

Overview
--------

The ITK WikiExamples can be built in several ways:

1. A single example. Follow the instrutions of the example.
2. All examples stand-alone. Follow `these instructions <http://www.vtk.org/Wiki/ITK/Examples/Instructions/ForUsers#Build_all_of_the_examples>`_.
3. All examples with a `Superbuild` that builds a proper ITK and VTK. Follow
   `these guidelines <http://www.vtk.org/Wiki/ITK/Examples/Instructions/ForUsers#Use_the_Superbuild_to_build_a_proper_VTK_and_ITK>`_.
4. As an ITK `Remote` Module. Follow these guidelines:

   * Enable the `Module_WikiExamples` boolean variable in your ITK CMake file.
   * Configure and generate your ITK project.
   * Build the `WikiExamples` modules.

If you want to test the `WikiExamples`, execute the follwoing commands::

  $ cd {itk_bld}
  $ cd Modules/Remote/WikiExamples
  $ ctest
