#ifndef LBT_VTK_IMPORT_UNITTEST
#define LBT_VTK_IMPORT_UNITTEST
#pragma once


/**\file     vtk_import_unittest.hpp
 * \mainpage Functions for unit testing voxelising geometries
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <type_traits>

#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include <gtest/gtest.h>

#include "../../src/geometry/vtk_import.hpp"


/**\class ImporterNewImageTest
 * \brief Helper class for setting up a test for allocating new image data
*/
class ImporterNewImageTest : public testing::Test {
  public:
    /**\fn        ImporterNewImageTest
     * \brief     (Default) constructor
     *
     * \param[in] resolution_x   The number of voxels in x-direction
     * \param[in] bounding_box   The type of bounding box to be used
     * \param[in] fill_colour    The colour that the new image should be filled with
    */
    ImporterNewImageTest(int const resolution_x = 100, std::array<double,6> const& bounding_box = {-1, 2, -3, 4, -5, 6},
                         char const fill_colour = lbt::Importer::background_colour) noexcept 
      : resolution_x{resolution_x}, bounding_box{bounding_box}, dimensions{0, 0, 0}, fill_colour{fill_colour} {
      image_data = lbt::Importer::newImageData(resolution_x, bounding_box, fill_colour);
      image_data->GetDimensions(dimensions);
      return;
    }
    ImporterNewImageTest(ImporterNewImageTest const&) = delete;
    ImporterNewImageTest(ImporterNewImageTest&&) = delete;
    ImporterNewImageTest& operator= (ImporterNewImageTest const&) = delete;
    ImporterNewImageTest& operator= (ImporterNewImageTest&&) = delete;
  protected:
    int resolution_x;
    std::array<double,6> bounding_box;
    int dimensions[3];
    char fill_colour;
    vtkSmartPointer<vtkImageData> image_data;
};

TEST_F(ImporterNewImageTest, validPointer) {
  EXPECT_NE(image_data.Get(), nullptr);
}

TEST_F(ImporterNewImageTest, dimensions) {
  int const expected_resolution_x {resolution_x};
  int const expected_resolution_y {static_cast<int>(resolution_x*(bounding_box[3]-bounding_box[2])/(bounding_box[1]-bounding_box[0]))};
  int const expected_resolution_z {static_cast<int>(resolution_x*(bounding_box[5]-bounding_box[4])/(bounding_box[1]-bounding_box[0]))};

  EXPECT_EQ(dimensions[0], expected_resolution_x);
  EXPECT_EQ(dimensions[1], expected_resolution_y);
  EXPECT_EQ(dimensions[2], expected_resolution_z);
}

TEST_F(ImporterNewImageTest, origin) {
  GTEST_SKIP() << "Test not implemented yet! Correct behaviour needs to be specified!";
}

TEST_F(ImporterNewImageTest, backgroundColour) {
  double const expected_colour {static_cast<double>(fill_colour)};
  
  for (int z = 0; z < dimensions[2]; ++z) {
    for (int y = 0; y < dimensions[1]; ++y) {
      for (int x = 0; x < dimensions[0]; ++x) {
        double const pixel {image_data->GetScalarComponentAsDouble(x,y,z,0)};
        EXPECT_DOUBLE_EQ(pixel, expected_colour);
      }
    }
  }
}

/// Test merging of two images
namespace lbt {
  namespace test {
    /**\class  MergeParameters
     * \brief  Class for holding all the testing parameters for a parametrised test
     * 
     * \tparam T   The type of the parameters
     * \tparam Dummy parameter used for SFINAE
    */
    template <typename T = char,
              typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    struct MergeParameters {
      public:
        T fill_colour_1;
        T fill_colour_2;
        T expected_colour;
    };
  }
}
/**\class ImporterMergeTest
 * \brief Helper class for setting up a parametrised image data merge test
*/
class ImporterMergeTest : public testing::Test, public testing::WithParamInterface<lbt::test::MergeParameters<char>> {
  public:
    /**\fn        ImporterMergeTest
     * \brief     (Default) constructor
     *
     * \param[in] resolution_x   The number of voxels in x-direction
     * \param[in] bounding_box   The type of bounding box to be used (either a manual bounding box or an automatic determined one)
    */
    ImporterMergeTest(int const resolution_x = 100, std::array<double,6> const& bounding_box = {-1, 2, -3, 4, -5, 6}) noexcept 
      : resolution_x{resolution_x}, bounding_box{bounding_box}, dimensions{0, 0, 0} {
      lbt::test::MergeParameters param {GetParam()};
      expected_colour = static_cast<double>(param.expected_colour);
      image_1 = lbt::Importer::newImageData(resolution_x, bounding_box, param.fill_colour_1);
      image_2 = lbt::Importer::newImageData(resolution_x, bounding_box, param.fill_colour_2);
  
      merged_image = lbt::Importer::mergeImageData(image_1, image_2);
      merged_image->GetDimensions(dimensions);
      return;
    }
    ImporterMergeTest(ImporterMergeTest const&) = delete;
    ImporterMergeTest(ImporterMergeTest&&) = delete;
    ImporterMergeTest& operator= (ImporterMergeTest const&) = delete;
    ImporterMergeTest& operator= (ImporterMergeTest&&) = delete;
  protected:
    int resolution_x;
    std::array<double,6> bounding_box;
    int dimensions[3];
    lbt::test::MergeParameters<char> parameters;
    double expected_colour;
    vtkSmartPointer<vtkImageData> image_1;
    vtkSmartPointer<vtkImageData> image_2;
    vtkSmartPointer<vtkImageData> merged_image;
};
TEST_P(ImporterMergeTest, mergedColour) {
  for (int z = 0; z < dimensions[2]; ++z) {
    for (int y = 0; y < dimensions[1]; ++y) {
      for (int x = 0; x < dimensions[0]; ++x) {
        double const pixel {merged_image->GetScalarComponentAsDouble(x,y,z,0)};
        EXPECT_DOUBLE_EQ(pixel, expected_colour);
      }
    }
  }
}
INSTANTIATE_TEST_SUITE_P(ImporterTest, ImporterMergeTest, testing::Values(
    lbt::test::MergeParameters<char>{lbt::Importer::background_colour, lbt::Importer::background_colour, lbt::Importer::background_colour},
    lbt::test::MergeParameters<char>{lbt::Importer::foreground_colour, lbt::Importer::foreground_colour, lbt::Importer::foreground_colour},
    lbt::test::MergeParameters<char>{lbt::Importer::foreground_colour, lbt::Importer::background_colour, lbt::Importer::foreground_colour},
    lbt::test::MergeParameters<char>{lbt::Importer::background_colour, lbt::Importer::foreground_colour, lbt::Importer::foreground_colour}
  )
);

TEST(ImporterTest, importFromFile) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(ImporterTest, load) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(ImporterTest, caveOutImageData) {
  constexpr int resolution_x {5};
  constexpr std::array<double,6> bounding_box = {-1, 1, -1, 1, -1, 1};
  constexpr char background_colour {lbt::Importer::background_colour};
  constexpr char foreground_colour {lbt::Importer::foreground_colour};
  vtkSmartPointer<vtkImageData> image_data = lbt::Importer::newImageData(resolution_x, bounding_box, background_colour);
  int dimensions[3] = {0, 0, 0};
  image_data->GetDimensions(dimensions);

  // Create rectangular domain
  for (int z = 0; z < dimensions[2]; ++z) {
    for (int y = 0; y < dimensions[1]; ++y) {
      for (int x = 0; x < dimensions[0]; ++x) {
        if ((x > 0 && x < 4) && (y > 0 && y < 4)) {
          image_data->SetScalarComponentFromDouble(x,y,z,0,foreground_colour);
        }
      }
    }
  }

  vtkSmartPointer<vtkImageData> caved_out_image_data {lbt::Importer::caveOutImageData(image_data)};

  // Test if inner pixels were removed
  // Potentially use pair of functors with initialisation and test criterion
  for (int z = 0; z < dimensions[2]; ++z) {
    for (int y = 0; y < dimensions[1]; ++y) {
      for (int x = 0; x < dimensions[0]; ++x) {
        double const pixel {caved_out_image_data->GetScalarComponentAsDouble(x,y,z,0)};
        if ((x > 0 && x < 4) && (y > 0 && y < 4) && (x != 2 && y != 2)) {
          EXPECT_DOUBLE_EQ(pixel, foreground_colour);
        } else {
          //EXPECT_DOUBLE_EQ(pixel, background_colour);
        }
      }
    }
  }
}

TEST(ImporterTest, loadObj) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(ImporterTest, loadPly) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(ImporterTest, loadStl) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(ImporterTest, loadVtk) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(ImporterTest, loadVtp) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(ImporterTest, rotatePolyData) {
  // Generate poly-data and rotate
  GTEST_SKIP() << "Unit test not implemented yet!";
}

TEST(ImporterTest, translatePolyData) {
  // Generate poly-data and move origin
  GTEST_SKIP() << "Unit test not implemented yet!";
}

TEST(ImporterTest, cleanPolyData) {
  // Generate cleanable poly-data and clean it
  GTEST_SKIP() << "Unit test not implemented yet!";
}

TEST(ImporterTest, reducePolyData) {
  // Create poly-data manually and reduce the complexity
  GTEST_SKIP() << "Unit test not implemented yet!";
}

TEST(ImporterTest, voxelisePolyData) {
  // Create poly-data manually and voxelise it
  GTEST_SKIP() << "Unit test not implemented yet!";
}

TEST(ImporterTest, cleanImageData) {
  // Create image data manually and clean it
  GTEST_SKIP() << "Unit test not implemented yet!";
}

namespace lbt {
  namespace test {

    /**\class GeometryConstructorTest
     * \brief Helper class for setting up a test for the geometry class
    */
    class GeometryConstructorTest : public testing::Test {
      public:
        /**\fn        GeometryConstructorTest
         * \brief     (Default) constructor
         *
         * \param[in] resolution_x   The number of voxels in x-direction
         * \param[in] bounding_box   The type of bounding box to be used
         * \param[in] fill_colour    The colour that the new image should be filled with
        */
        GeometryConstructorTest(int const resolution_x = 100, std::array<double,6> const& bounding_box = {-1, 2, -3, 4, -5, 6},
                            char const fill_colour = lbt::Importer::background_colour) noexcept 
          : resolution_x{resolution_x}, bounding_box{bounding_box}, fill_colour{fill_colour} {
          image_data = lbt::Importer::newImageData(resolution_x, bounding_box, fill_colour);
          return;
        }
        GeometryConstructorTest(GeometryConstructorTest const&) = delete;
        GeometryConstructorTest(GeometryConstructorTest&&) = delete;
        GeometryConstructorTest& operator= (GeometryConstructorTest const&) = delete;
        GeometryConstructorTest& operator= (GeometryConstructorTest&&) = delete;
      protected:
        int resolution_x;
        std::array<double,6> bounding_box;
        char fill_colour;
        vtkSmartPointer<vtkImageData> image_data;
    };

    /**\class Geometry
     * \brief Helper class for unit testing private member variables
    */
    class Geometry : public lbt::Geometry {
      protected:
        friend class GeometryConstructorTest;
        FRIEND_TEST(GeometryConstructorTest, constructorFromImageData);
        FRIEND_TEST(GeometryConstructorTest, copyConstructor);
        FRIEND_TEST(GeometryConstructorTest, moveConstructor);
        FRIEND_TEST(GeometryConstructorTest, copyAssignment);
        FRIEND_TEST(GeometryConstructorTest, moveAssignment);
    };

    TEST_F(GeometryConstructorTest, constructorFromImageData) {
      lbt::test::Geometry geometry {image_data};
      EXPECT_EQ(image_data.Get(), geometry.image_data.Get());
    }

    TEST_F(GeometryConstructorTest, copyConstructor) {
      lbt::test::Geometry geometry_1 {image_data};
      lbt::test::Geometry geometry_2 {geometry_1};
      EXPECT_NE(geometry_1.image_data.Get(), geometry_2.image_data.Get());
    }

    TEST_F(GeometryConstructorTest, moveConstructor) {
      lbt::test::Geometry geometry_1 {image_data};
      auto const geometry_1_ptr = geometry_1.image_data.Get();
      lbt::test::Geometry geometry_2 {std::move(geometry_1)};
      EXPECT_EQ(geometry_1_ptr, geometry_2.image_data.Get());
    }

    TEST_F(GeometryConstructorTest, copyAssignment) {
      lbt::test::Geometry geometry_1 {image_data};
      lbt::test::Geometry geometry_2 = geometry_1;
      EXPECT_NE(geometry_1.image_data.Get(), geometry_2.image_data.Get());
    }

    TEST_F(GeometryConstructorTest, moveAssignment) {
      lbt::test::Geometry geometry_1 {image_data};
      auto const geometry_1_ptr = geometry_1.image_data.Get();
      lbt::test::Geometry geometry_2 = std::move(geometry_1);
      EXPECT_EQ(geometry_1_ptr, geometry_2.image_data.Get());
    }
  }
}

TEST(GeometryTest, importFromFiles) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(GeometryTest, importFromMhd) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(GeometryTest, exportToMhd) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(GeometryTest, exportToVtk) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

TEST(GeometryTest, exportToFile) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

#endif // LBT_VTK_IMPORT_UNITTEST
