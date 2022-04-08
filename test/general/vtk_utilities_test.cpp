/**
 * \file     vtk_utilities_test.cpp
 * \mainpage Unit tests for handling geometries with VTK
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include "../../src/general/use_vtk.hpp"

#ifdef LBT_USE_VTK
  #include <gtest/gtest.h>

  #include "../../src/general/vtk_utilities.hpp"


  namespace lbt {
    namespace test {

      TEST(VtkUtilities, exportImageDataToVtk) {
        GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
      }

      TEST(VtkUtilities, exportImageDataToMhd) {
        GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
      }

    }
  }
#endif // LBT_USE_VTK
