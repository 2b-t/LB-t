#ifndef LBT_VTK_UTILITITES
#define LBT_VTK_UTILITITES
#pragma once


/**\file     vtk_utilities.hpp
 * \mainpage Functions for handling geometries with VTK
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <filesystem>
#include <string>

#if __has_include (<vtkSmartPointer.h>)
  #include <vtkImageData.h>
  #include <vtkSmartPointer.h>


  namespace lbt {

    /// Enum class for supported data types
    enum class DataType {VTK, MHD};

    /**\fn        saveImageDataToVtk
     * \brief     Export a scalar to a *.vtk-file
     *
     * \param[in] image_data    The meta-image data to be allocated
     * \param[in] output_path   The output path where the file should be saved
     * \param[in] filename      The filename of the exported file without the file ending
    */
    void saveImageDataToVtk(vtkSmartPointer<vtkImageData> const& image_data, 
                            std::filesystem::path const& output_path, std::string const& filename) noexcept;

    /**\fn        saveImageDataToMhd
     * \brief     Export a scalar to a meta-image *.mhd-file
     *
     * \param[in] image_data    The meta-image data to be allocated
     * \param[in] filename      The filename of the exported file without the file ending
     * \param[in] output_path   The output path where the file should be saved
     * \param[in] is_compress   Boolean variable signaling whether the output should be compressed or not
    */
    void saveImageDataToMhd(vtkSmartPointer<vtkImageData> const& image_data, 
                            std::filesystem::path const& output_path, std::string const& filename, bool const is_compress) noexcept;
  }
#endif

#endif // LBT_VTK_UTILITITES
