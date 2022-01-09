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
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include "../general/vtk_utilities.hpp"


namespace lbt {

    /// Enum class for geometric axes
  enum class Axis {x, y, z};

  /**\class Importer
   * \brief Static class containing different functions for importing and processing VTK files
  */
  class Importer {
    public:
      Importer(Importer const&) = delete;
      Importer(Importer&&) = delete;
      Importer& operator= (Importer const&) = delete;
      Importer& operator= (Importer&&) = delete;

      /**\fn        newImageData
       * \brief     Function for creating new image data with a given resolution and given bounding box
       *
       * \param[in] resolution_x   The number of voxels in x-direction
       * \param[in] bounding_box   The type of bounding box to be used (either a manual bounding box or an automatic determined one)
       * \param[in] fill_colour    The colour that the new image should be filled with, defaults to the background colour
       * \return    An empty image-data object filled with the background colour
      */
      static vtkSmartPointer<vtkImageData> newImageData(int const resolution_x, std::array<double, 6> const& bounding_box,
                                                        char const fill_colour = Importer::background_colour) noexcept;

      /**\fn        mergeImageData
       * \brief     Function for merging image data objects
       *
       * \param[in] image_data_1   Image-data to be merged
       * \param[in] image_data_2   Image-data to be merged
       * \return    Image-data obtained by merging \p image_data_1 and \p image_data_2
      */
      static vtkSmartPointer<vtkImageData> mergeImageData(vtkSmartPointer<vtkImageData> const& image_data_1,
                                                          vtkSmartPointer<vtkImageData> const& image_data_2) noexcept;
    
      /**\fn        importFromFile
       * \brief     Function for importing and voxelising an OBJ/STL/PLY/VTK/VTP file
       *            Source: https://vtk.org/Wiki/VTK/Examples/Cxx/PolyData/PolyDataToImageData
       *
       * \param[in] filename         The filename of the OBJ, STL, PLY, VTK or VTP file
       * \param[in] resolution_x     The number of voxels in x-direction
       * \param[in] reduction_rate   The rate at which the poly-data should be reduced (ignored for VTK!)
       * \param[in] bounding_box     The type of bounding box to be used (either a manual bounding box or an automatic determined one)
       * \return    The corresponding domain with regular grid as VTK image data
      */
      static vtkSmartPointer<vtkImageData> importFromFile(std::filesystem::path const& filename, int const resolution_x, double const reduction_rate, 
                                                          std::optional<std::array<double, 6>> const& opt_bounding_box) noexcept;

      /**\fn        load
       * \brief     Function for loading an OBJ, PLY, STL, VTK and VTP files
       *
       * \param[in] filename   The filename of the OBJ, PLY, STL, VTK or VTP file to be loaded including the file-extension
       * \return    The loaded poly-data
      */
      static vtkSmartPointer<vtkPolyData> load(std::filesystem::path const& filename) noexcept;

      /**\fn        caveOutImageData
       * \brief     Function for caving out image-data object: Pixels that do not have a background pixel as neighbour are set to the 
       *            background colour
       *            Sources: https://kitware.github.io/vtk-examples/site/Cxx/Images/ImageSeparableConvolution/
       *                     https://kitware.github.io/vtk-examples/site/Cxx/Images/ImageThreshold/
       *                     https://itk.org/Doxygen413/html/Examples_2Visualization_2CannyEdgeDetectionImageFilterConnectVTKITK_8py-example.html
       *
       * \param[in] image_data   The image data to be caved out
       * \return    The caved-out image-data object
      */
      static vtkSmartPointer<vtkImageData> caveOutImageData(vtkSmartPointer<vtkImageData> image_data) noexcept;

      static constexpr unsigned char background_colour {0};
      static constexpr unsigned char foreground_colour {1};
      static constexpr auto scalar_data_type {VTK_UNSIGNED_CHAR};

    protected:
      /// For unit testing
      Importer() = default;

      /**\fn        loadObj
       * \brief     Function for loading an OBJ file
       *            Source: https://vtk.org/Wiki/VTK/Examples/Cxx/IO/ReadOBJ
       *
       * \param[in] filename   The filename of the OBJ file to be loaded including the file-extension
       * \return    The loaded poly-data
      */
      static vtkSmartPointer<vtkPolyData> loadObj(std::filesystem::path const& filename) noexcept;
      
      /**\fn        loadPly
       * \brief     Function for loading a PLY file
       *
       * \param[in] filename   The filename of the PLY file to be loaded including the file-extension
       * \return    The loaded poly-data
      */
      static vtkSmartPointer<vtkPolyData> loadPly(std::filesystem::path const& filename) noexcept;

      /**\fn        loadStl
       * \brief     Function for importing an STL file
       *            Source: https://vtk.org/Wiki/VTK/Examples/Cxx/IO/ReadSTL
       *
       * \param[in] filename   The filename of the STL file to be loaded including the file-extension
       * \return    The loaded poly-data
      */
      static vtkSmartPointer<vtkPolyData> loadStl(std::filesystem::path const& filename) noexcept;

      /**\fn        loadVtk
       * \brief     Function for loading a VTK file
       *
       * \param[in] filename   The filename of the VTK file to be loaded including the file-extension
       * \return    The loaded poly-data
      */
      static vtkSmartPointer<vtkPolyData> loadVtk(std::filesystem::path const& filename) noexcept;

      /**\fn        loadVtp
       * \brief     Function for loading a VTP file
       *
       * \param[in] filename   The filename of the VTP file to be loaded including the file-extension
       * \return    The loaded poly-data
      */
      static vtkSmartPointer<vtkPolyData> loadVtp(std::filesystem::path const& filename) noexcept;

      /**\fn        rotatePolyData
       * \brief     Function for rotating a poly-data object around its origin
       *            Source: https://kitware.github.io/vtk-examples/site/Cxx/PolyData/RotationAroundLine/
       *
       * \param[in] input_poly_data   The poly-data object to be transformed
       * \param[in] rotation_angle    The rotation angle in degrees which should be used to transform th existing poly-data object
       * \param[in] rotation_axis     The corresponding rotation axis around which the object should be rotated
       * \return    The rotated poly-data object
      */
      static vtkSmartPointer<vtkPolyData> rotatePolyData(vtkSmartPointer<vtkPolyData> input_poly_data, double const rotation_angle, 
                                                         Axis const rotation_axis = Axis::z) noexcept;

      /**\fn        translatePolyData
       * \brief     Function for translating a poly-data object
       *            Source: https://vtk.org/doc/nightly/html/classvtkTransform.html#ad5a571f4a6c78cd7e374b381ea438638
       *
       * \param[in] input_poly_data   The poly-data object to be transformed
       * \param[in] translation       The desired translation by which the poly-data should be translated {x, y, z}
       * \return    The translated poly-data object
      */
      static vtkSmartPointer<vtkPolyData> translatePolyData(vtkSmartPointer<vtkPolyData> input_poly_data, 
                                                            std::array<double,3> const& translation) noexcept;

      /**\fn        cleanPolyData
       * \brief     Function for cleaning a poly-data object
       *
       * \param[in] poly_data   The poly-data object to be cleaned
       * \return    The cleaned poly-data object
      */
      static vtkSmartPointer<vtkPolyData> cleanPolyData(vtkSmartPointer<vtkPolyData> poly_data) noexcept;
      
      /**\fn        reducePolyData
       * \brief     Function for reducing the polygons of a poly-data object
       *
       * \param[in] poly_data        The poly-data object whose polygons should be reduced
       * \param[in] reduction_rate   The rate by which the polygons should be reduced
       * \return    The reduced poly-data object
      */
      static vtkSmartPointer<vtkPolyData> reducePolyData(vtkSmartPointer<vtkPolyData> poly_data, 
                                                         double const reduction_rate) noexcept;

      /**\fn        voxelisePolyData
       * \brief     Function for voxelising poly-data
       *            Source: https://vtk.org/Wiki/VTK/Examples/Cxx/PolyData/PolyDataToImageData
       *
       * \param[in] poly_data          The poly-data to be voxelised
       * \param[in] resolution_x       The number of voxels in the x-direction
       * \param[in} opt_bounding_box   The optional bounding box to consider
       * \return    The created voxels as image-data object
      */
      static vtkSmartPointer<vtkImageData> voxelisePolyData(vtkSmartPointer<vtkPolyData> const& poly_data, int const resolution_x,
                                                            std::optional<std::array<double, 6>> const& opt_bounding_box) noexcept;

      /**\fn        cleanImageData
       * \brief     Function for cleaning the image-data object by applying filter operations such as dilation, erosion, 
       *            opening and closing
       *
       * \param[in] image_data   The image data to be cleaned
       * \return    The cleaned image-data object
      */
      static vtkSmartPointer<vtkImageData> cleanImageData(vtkSmartPointer<vtkImageData> image_data) noexcept;
  };


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
        this->image_data = vtkSmartPointer<vtkImageData>::New();
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
        this->image_data = vtkSmartPointer<vtkImageData>::New();
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

      /**\fn        saveToMhd
       * \brief     Function for exporting a geometry to a MHD file
       *
       * \param[in] output_path   The output path where the file should be saved
       * \param[in] filename      The filename without the file ending
       * \param[in] is_compress   Boolean flag signaling whether the file should be compressed or not
       * \return    None
      */
      void saveToMhd(std::filesystem::path const& output_path, std::string const& filename, bool const is_compress) const noexcept;

      /**\fn        saveToVtk
       * \brief     Function for exporting geometry to a VTK file
       *
       * \param[in] output_path   The output path where the file should be saved
       * \param[in] filename      The filename without the file ending
       * \return    None
      */
      void saveToVtk(std::filesystem::path const& output_path, std::string const& filename) const noexcept;

      /**\fn        saveToFile
       * \brief     Function for exporting geometry to a MHD (default) or VTK file
       *
       * \param[in] output_path   The output path where the file should be saved
       * \param[in] filename      The filename without the file ending
       * \param[in] data_type     The output data format (either MHD or VTK)
       * \param[in] is_compress   Boolean flag signaling whether the file should be compressed or not
       * \return    None
      */
      void saveToFile(std::filesystem::path const& output_path, std::string const& filename, 
                      DataType const data_type = DataType::MHD, bool const is_compress = true) const noexcept;

    protected:
      vtkSmartPointer<vtkImageData> image_data;
  };

}

#endif // LBT_VTK_IMPORT
