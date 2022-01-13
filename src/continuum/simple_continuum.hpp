#ifndef LBT_SIMPLE_CONTINUUM
#define LBT_SIMPLE_CONTINUUM
#pragma once

/**
 * \file     simple_continuum.hpp
 * \mainpage Legacy class for continuum properties without any external library
 *           The population is exported in the VTK format. This is comparably slow but allows somebody to compile the code without VTK!
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <tuple>
#include <type_traits>

#include "../general/output_utilities.hpp"
#include "../general/type_definitions.hpp"
#include "continuum_base.hpp"


namespace lbt {

  /**\class  SimpleContinuum
   * \brief  Legacy class for the macroscopic variables that does not require any external library
   *
   * \tparam T   Floating data type used for simulation
  */
  template <typename T>
  class SimpleContinuum : public ContinuumBase<T> {
    static_assert(std::is_floating_point_v<T>, "Invalid template parameter 'T'.");

    public:
      SimpleContinuum() = delete;

      /**\fn    SimpleContinuum
       * \brief Class constructor
       * 
       * \param[in] NX            Simulation domain resolution in x-direction
       * \param[in] NY            Simulation domain resolution in y-direction
       * \param[in] NZ            Simulation domain resolution in z-direction
       * \param[in] output_path   The path where the output files should be written to
      */
      SimpleContinuum(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ, std::filesystem::path const& output_path) noexcept
        : ContinuumBase<T>{NX, NY, NZ, output_path}, memory_size{static_cast<std::size_t>(NZ)*NY*NX*number_of_values}, M(memory_size) {
        return;
      }
      SimpleContinuum(SimpleContinuum&) = default;
      SimpleContinuum& operator = (SimpleContinuum&) = default;
      SimpleContinuum(SimpleContinuum&&) = default;
      SimpleContinuum& operator = (SimpleContinuum&&) = default;

      void setP(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept override;
      void setU(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept override;
      void setV(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept override;
      void setW(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept override;
      T getP(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept override;
      T getU(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept override;
      T getV(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept override;
      T getW(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept override;
      void save(double const timestamp) const noexcept override;

      /**\fn        operator()
       * \brief     Access scalar values at given coordinates
       *
       * \param[in] x   x coordinate of cell
       * \param[in] y   y coordinate of cell
       * \param[in] z   z coordinate of cell
       * \param[in] m   Macroscopic value (0: density, 1-3: ux, uy, uz)
       * \return    The requested scalar value
      */
      inline T&       operator() (std::int32_t const x, std::int32_t const y, std::int32_t const z, std::int32_t const m) noexcept;
      inline T const& operator() (std::int32_t const x, std::int32_t const y, std::int32_t const z, std::int32_t const m) const noexcept;

      /**\fn        saveToVtk
       * \brief     Export velocity and density at current time step to a *.vtk-file that can then be
       *            read by visualisation applications like ParaView.
       * \warning   *.vtk export is comparably slow and large! This export is furthermore significantly slower than with the 
       *            VTK-library
       *
       * \param[in] timestamp   The current time stamp that will be used for the name
      */
      void saveToVtk(double const timestamp) const noexcept;

      /**\fn        saveToBin
       * \brief     Export any scalar quantity at current time step to *.bin file writing to *.bin-files
       *            is significantly faster than using non-binary *.txt-files
       *
       * \param[in] name        The export file name of the scalar
       * \param[in] timestamp   The current time step that will be used for the name
      */
      void saveToBin(double const timestamp) const noexcept;

      /**\fn        loadFromBin
       * \brief     Import macroscopic values from *.bin-file
       *
       * \param[in] name        The import file name holding the macroscopic quantities
       * \param[in] timestamp   The current time step that will be used for the name
      */
      void loadFromBin(std::string const& name, double const timestamp) noexcept;

    protected:
      /**\fn        spatialToLinear_
       * \brief     Inline function for converting 3D population coordinates to scalar index
       * \warning   Inline function! Has to be declared in header!
       *
       * \param[in] x   x coordinate of cell
       * \param[in] y   y coordinate of cell
       * \param[in] z   z coordinate of cell
       * \param[in] m   Macroscopic value (0: density, 1-3: ux, uy, uz)
       * \return    Requested linear scalar index
      */
      inline std::int64_t spatialToLinear_(std::int32_t const x, std::int32_t const y, std::int32_t const z,
                                           std::int32_t const m) const noexcept;

      /**\fn         linearToSpatial_
       * \brief      Generate 3D population coordinates from scalar index
       *
       * \param[in]  index   Current linear scalar index
       * \return     Return value containing the x, y and z coordinates as well as the number of macroscopic values belonging to the scalar index
      */
      std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t> linearToSpatial_(std::int64_t const index) const noexcept;

      /**\fn        saveScalarToVtk_
       * \brief     Export arbitrary scalar at current time step to a *.vtk-file that can then be read by visualisation applications like ParaView.
       * \warning   *.vtk export is comparably slow! Better export to .bin-files and then convert them to *.vtk-files with the external converter app 
       *            afterwards.
       *
       * \param[in] m             Index of the scalar to be exported
       * \param[in] output_path   The output path where the resulting file should be exported to
       * \param[in] filename      The name of the file
      */
      void saveScalarToVtk_(std::int32_t const m, std::filesystem::path const& output_path, std::string const& filename) const noexcept;

      /**\fn        saveToSingleVtk_
       * \brief     Export current time step to a *.vtk-file that can then be read by visualisation applications like ParaView.
       * \warning   *.vtk export is comparably slow! Better export to .bin-files and then convert them to *.vtk-files with the external converter app 
       *            afterwards.
       *
       * \param[in] output_path   The output path where the resulting file should be exported to
       * \param[in] filename      The name of the file
      */
      void saveToSingleVtk_(std::filesystem::path const& output_path, std::string const& filename) const noexcept;

      static constexpr std::int32_t number_of_values = 4; // Number of macroscopic values: rho, ux, uy, uz
      std::size_t memory_size; // Size of the array in Bytes
      lbt::HeapArray<T> M; // Heap-allocated array
  };

  template <typename T>
  void SimpleContinuum<T>::setP(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    (*this)(x,y,z,0) = value;
    return;
  }

  template <typename T>
  void SimpleContinuum<T>::setU(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    (*this)(x,y,z,1) = value;
    return;
  }

  template <typename T>
  void SimpleContinuum<T>::setV(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    (*this)(x,y,z,2) = value;
    return;
  }

  template <typename T>
  void SimpleContinuum<T>::setW(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    (*this)(x,y,z,3) = value;
    return;
  }

  template <typename T>
  T SimpleContinuum<T>::getP(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    return (*this)(x,y,z,0);
  }

  template <typename T>
  T SimpleContinuum<T>::getU(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    return (*this)(x,y,z,1);
  }

  template <typename T>
  T SimpleContinuum<T>::getV(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    return (*this)(x,y,z,2);
  }

  template <typename T>
  T SimpleContinuum<T>::getW(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    return (*this)(x,y,z,3);
  }

  template <typename T>
  void SimpleContinuum<T>::save(double const timestamp) const noexcept {
    saveToVtk(timestamp);
    return;
  }

  template <typename T>
  inline T& SimpleContinuum<T>::operator() (std::int32_t const x, std::int32_t const y, std::int32_t const z,
                                           std::int32_t const m) noexcept {
    return M[spatialToLinear_(x,y,z,m)];
  }

  template <typename T>
  inline T const& SimpleContinuum<T>::operator() (std::int32_t const x, std::int32_t const y, std::int32_t const z,
                                                 std::int32_t const m) const noexcept {
    return M[spatialToLinear_(x,y,z,m)];
  }

  template <typename T>
  void SimpleContinuum<T>::saveToVtk(double const timestamp) const noexcept {
    std::string const filename_p {"p_" + toString(timestamp)}; 
    saveScalarToVtk_(0, ContinuumBase<T>::output_path, filename_p);
    std::string const filename_u {"u_" + toString(timestamp)}; 
    saveScalarToVtk_(1, ContinuumBase<T>::output_path, filename_u);
    std::string const filename_v {"v_" + toString(timestamp)}; 
    saveScalarToVtk_(2, ContinuumBase<T>::output_path, filename_v);
    std::string const filename_w {"w_" + toString(timestamp)}; 
    saveScalarToVtk_(3, ContinuumBase<T>::output_path, filename_w);
    return;
  }

  template <typename T>
  void SimpleContinuum<T>::saveToBin(double const timestamp) const noexcept {
    std::filesystem::create_directories(ContinuumBase<T>::output_path);
    std::filesystem::path const filename_with_extension = ContinuumBase<T>::output_path / std::string{"step_" + toString(timestamp) + ".bin"};
    auto const export_file = std::unique_ptr<FILE, decltype(&std::fclose)>(std::fopen(filename_with_extension.c_str(), "wb+"), &std::fclose);

    if (export_file != nullptr) {
      std::fwrite(M, 1, memory_size, export_file.get());
    } else {
      std::cerr << "Fatal Error: File '" << filename_with_extension.string() << "' could not be opened." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    return;
  }

  template <typename T>
  void SimpleContinuum<T>::loadFromBin(std::string const& name, double const timestamp) noexcept {
    std::filesystem::create_directories(ContinuumBase<T>::output_path);
    std::filesystem::path const filename_with_extension = ContinuumBase<T>::output_path / std::string{name + "_" + toString(timestamp) + ".bin"};
    auto const import_file = std::unique_ptr<FILE, decltype(&std::fclose)>(std::fopen(filename_with_extension.c_str(), "rb+"), &std::fclose);

    if(import_file != nullptr) {
      std::fread(M, 1, memory_size, import_file.get());
    } else {
      std::cerr << "Fatal Error: File '" << filename_with_extension.string() << "' could not be opened." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    return;
  }

  template <typename T>
  inline std::int64_t SimpleContinuum<T>::spatialToLinear_(std::int32_t const x, std::int32_t const y, std::int32_t const z,
                                                     std::int32_t const m) const noexcept {
    return ((static_cast<std::int64_t>(z)*ContinuumBase<T>::NY + y)*ContinuumBase<T>::NX + x)*number_of_values + m;
  }

  template <typename T>
  std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t>
  SimpleContinuum<T>::linearToSpatial_(std::int64_t const index) const noexcept {
    std::int64_t factor {number_of_values*ContinuumBase<T>::NX*ContinuumBase<T>::NY};
    std::int64_t rest {index%factor};

    std::int32_t const z {static_cast<std::int32_t>(index/factor)};

    factor = number_of_values*ContinuumBase<T>::NX;
    std::int32_t const y {static_cast<std::int32_t>(rest/factor)};
    rest = rest%factor;

    factor = number_of_values;
    std::int32_t const x {static_cast<std::int32_t>(rest/factor)};
    std::int32_t const m {static_cast<std::int32_t>(rest%factor)};

    return std::make_tuple(x,y,z,m);
  }

  template <typename T>
  void SimpleContinuum<T>::saveScalarToVtk_(std::int32_t const m, std::filesystem::path const& output_path, std::string const& filename) const noexcept {
    std::filesystem::create_directories(ContinuumBase<T>::output_path);
    std::filesystem::path const filename_with_extension = ContinuumBase<T>::output_path / std::string{filename + ".vtk"};
    auto const export_file = std::unique_ptr<FILE, decltype(&std::fclose)>(std::fopen(filename_with_extension.c_str(), "w"), &std::fclose);

    if (export_file != nullptr) {
      std::fprintf(export_file.get(), "# vtk DataFile Version 3.0\n");
      std::fprintf(export_file.get(), "LBM CFD simulation scalar\n");
      std::fprintf(export_file.get(), "ASCII\n");
      std::fprintf(export_file.get(), "DATASET STRUCTURED_POINTS\n");

      std::fprintf(export_file.get(), "DIMENSIONS %" PRId32 " %" PRId32 " %" PRId32 "\n", ContinuumBase<T>::NX+1, ContinuumBase<T>::NY+1, ContinuumBase<T>::NZ+1);
      std::fprintf(export_file.get(), "SPACING 1 1 1\n");
      std::fprintf(export_file.get(), "ORIGIN 0 0 0\n");

      std::int64_t const number_of_cells = static_cast<std::int64_t>(ContinuumBase<T>::NX)*ContinuumBase<T>::NY*ContinuumBase<T>::NZ;
      std::fprintf(export_file.get(), "CELL_DATA %" PRId64 "\n", number_of_cells);

      std::fprintf(export_file.get(), "SCALARS transport_scalar float 1\n");
      std::fprintf(export_file.get(), "LOOKUP_TABLE default\n");
      for(std::int32_t z = 0; z < ContinuumBase<T>::NZ; ++z) {
        for(std::int32_t y = 0; y < ContinuumBase<T>::NY; ++y) {
          for(std::int32_t x = 0; x < ContinuumBase<T>::NX; ++x) {
            std::fprintf(export_file.get(), "%f\n", (*this)(x, y, z, m));
          }
        }
      }
    } else {
      std::cerr << "Fatal Error: File '" << filename_with_extension.string() << "' could not be opened." << std::endl;
      std::exit(EXIT_FAILURE);
    }
    return;
  }

  template <typename T>
  void SimpleContinuum<T>::saveToSingleVtk_(std::filesystem::path const& output_path, std::string const& filename) const noexcept {
    std::filesystem::create_directories(ContinuumBase<T>::output_path);
    std::filesystem::path const filename_with_extension = ContinuumBase<T>::output_path / std::string{filename + ".vtk"};
    auto const export_file = std::unique_ptr<FILE, decltype(&std::fclose)>(std::fopen(filename_with_extension.c_str(), "w"), &std::fclose);

    if (export_file != nullptr) {
      std::fprintf(export_file.get(), "# vtk DataFile Version 3.0\n");
      std::fprintf(export_file.get(), "LBM CFD simulation velocity\n");
      std::fprintf(export_file.get(), "ASCII\n");
      std::fprintf(export_file.get(), "DATASET STRUCTURED_POINTS\n");

      std::fprintf(export_file.get(), "DIMENSIONS %" PRId32 " %" PRId32 " %" PRId32 "\n", ContinuumBase<T>::NX+1, ContinuumBase<T>::NY+1, ContinuumBase<T>::NZ+1);
      std::fprintf(export_file.get(), "SPACING 1 1 1\n");
      std::fprintf(export_file.get(), "ORIGIN 0 0 0\n");

      std::int64_t const number_of_cells = static_cast<std::int64_t>(ContinuumBase<T>::NX)*ContinuumBase<T>::NY*ContinuumBase<T>::NZ;
      std::fprintf(export_file.get(), "CELL_DATA %" PRId64 "\n", number_of_cells);

      std::fprintf(export_file.get(), "SCALARS density_variation float 1\n");
      std::fprintf(export_file.get(), "LOOKUP_TABLE default\n");
      for(std::int32_t z = 0; z < ContinuumBase<T>::NZ; ++z) {
        for(std::int32_t y = 0; y < ContinuumBase<T>::NY; ++y) {
          for(std::int32_t x = 0; x < ContinuumBase<T>::NX; ++x) {
            std::fprintf(export_file.get(), "%f\n", getP(x, y, z));
          }
        }
      }

      std::fprintf(export_file.get(), "VECTORS velocity_vector float\n");
      for(std::int32_t z = 0; z < ContinuumBase<T>::NZ; ++z) {
        for(std::int32_t y = 0; y < ContinuumBase<T>::NY; ++y) {
          for(std::int32_t x = 0; x < ContinuumBase<T>::NX; ++x) {
            std::fprintf(export_file.get(), "%f %f %f\n", getU(x, y, z),
                                                          getV(x, y, z),
                                                          getW(x, y, z));
          }
        }
      }
    } else {
      std::cerr << "Fatal Error: File '" << filename_with_extension.string() << "' could not be opened." << std::endl;
      std::exit(EXIT_FAILURE);
    }
    return;
  }
}

#endif // LBT_PLAIN_CONTINUUM
