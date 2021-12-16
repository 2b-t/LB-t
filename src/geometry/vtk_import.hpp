#ifndef LBT_VTK_IMPORT
#define LBT_VTK_IMPORT
#pragma once


/**\file     vtk_import.hpp
 * \mainpage Functions for voxelising geometries given by a STL or OBJ file for further processing 
 *           in the LBM simulation.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <array>
#include <cstdlib>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <vtkImageData.h>
#include <vtkSmartPointer.h>


namespace lbt {
  
  /// Enum class for supported data types
  enum class DataType {VTK, MHD};

  /**\class  Geometry
   * \brief  Class containing a Geometry as image-data
  */
  class Geometry {
    public:
      /**\fn        Geometry
       * \brief     (Default) constructor of geometry from image-data
       *
       * \param[in] image_data   The image-data that the geometry should be created from
       * \return    None
      */
      Geometry(vtkSmartPointer<vtkImageData> image_data = vtkSmartPointer<vtkImageData>::New()) noexcept
        : image_data{image_data} {
        return;
      }

      /**\fn        Geometry
       * \brief     Copy constructor
       *
       * \param[in] geometry   Another geometry
       * \return    None
      */
      Geometry(Geometry const& geometry) noexcept {
        this->image_data->DeepCopy(geometry.image_data);
        return;
      }

      /**\fn        Geometry
       * \brief     Move constructor
       *
       * \param[in] geometry   Another geometry
       * \return    None
      */
      Geometry(Geometry&& geometry) noexcept
        : image_data{geometry.image_data} {
        return;
      }

      /**\fn        Geometry
       * \brief     Copy assignment operator
       *
       * \param[in] geometry   Another geometry
       * \return    The copied geometry
      */
      Geometry& operator= (Geometry const& geometry) noexcept {
        this->image_data->DeepCopy(geometry.image_data);
        return *this;
      }

      /**\fn        Geometry
       * \brief     Move assignment operator
       *
       * \param[in] geometry   Another geometry
       * \return    The new geometry
      */
      Geometry& operator= (Geometry&& geometry) noexcept {
        this->image_data = geometry.image_data;
        return *this;
      }

      /**\fn        importFromFiles
       * \brief     Function for importing and voxelising several OBJ/STL/PLY/VTK/VTP files and 
       *            merging them to a single geometry
       *            Sources: https://kitware.github.io/vtk-examples/site/Cxx/Images/ImageMathematics/
       *                     https://vtk.org/doc/nightly/html/classvtkImageMathematics.html
       *
       * \param[in] filename             The filename of the OBJ, STL, PLY, VTK or VTP file
       * \param[in] image_resolution_x   The resolution of the voxel domain
       * \param[in] reduction_rate       The rate at which the poly-data should be reduced (ignored for VTK!)
       * \param[in] opt_bounding_box     The optional bounding box {x_min, x_max, y_min, x_max, z_min, z_max}
       * \return    The corresponding geometry
      */
      static Geometry importFromFiles(std::vector<std::filesystem::path> const& filenames, int const resolution_x, double const reduction_rate = 0.0,
                                      std::optional<std::array<double, 6>> const& opt_bounding_box = std::nullopt) noexcept;

      /**\fn        importFromMhd
       * \brief     Function for importing a geometry from a MHD file
       *            Source: https://kitware.github.io/vtk-examples/site/Cxx/IO/MetaImageReader/
       *
       * \param[in] filename      The filename including the file ending
       * \return    The parsed geometry
      */
      static Geometry importFromMhd(std::filesystem::path const& filename) noexcept;

      /**\fn        exportToMhd
       * \brief     Function for exporting a geometry to a MHD file
       *
       * \param[in] output_path   The output path where the file should be saved
       * \param[in] filename      The filename without the file ending
       * \param[in] is_compress   Boolean flag signaling whether the file should be compressed or not
       * \return    None
      */
      void exportToMhd(std::filesystem::path const& output_path, std::string const& filename, bool const is_compress) const noexcept;

      /**\fn        exportToVtk
       * \brief     Function for exporting geometry to a VTK file
       *
       * \param[in] output_path   The output path where the file should be saved
       * \param[in] filename      The filename without the file ending
       * \return    None
      */
      void exportToVtk(std::filesystem::path const& output_path, std::string const& filename) const noexcept;

      /**\fn        exportToFile
       * \brief     Function for exporting geometry to a MHD (default) or VTK file
       *
       * \param[in] output_path   The output path where the file should be saved
       * \param[in] filename      The filename without the file ending
       * \param[in] data_type     The output data format (either MHD or VTK)
       * \param[in] is_compress   Boolean flag signaling whether the file should be compressed or not
       * \return    None
      */
      void exportToFile(std::filesystem::path const& output_path, std::string const& filename, DataType const data_type = DataType::MHD, bool const is_compress = true) const noexcept;

    protected:
      vtkSmartPointer<vtkImageData> image_data;
  };

}

#endif // LBT_VTK_IMPORT
