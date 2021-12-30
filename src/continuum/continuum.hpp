#ifndef LBT_CONTINUUM
#define LBT_CONTINUUM

/**
 * \file     continuum.hpp
 * \mainpage Class for continuum properties
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <string>
#include <type_traits>

#include <vtkDataArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>

#include "../general/output_utilities.hpp"
#include "../general/vtk_utilities.hpp"


namespace lbt {
  /**\class  Continuum
   * \brief  Class for the macroscopic variables
   *
   * \tparam T    Floating data type used for simulation
  */
  template <typename T>
  class Continuum {
    public:
      Continuum() = delete;
      Continuum& operator = (Continuum&) = delete;
      Continuum(Continuum&&) = delete;
      Continuum& operator = (Continuum&&) = delete;

      /**\brief Class constructor
       * 
       * \param[in] NX            Simulation domain resolution in x-direction
       * \param[in] NY            Simulation domain resolution in y-direction
       * \param[in] NZ            Simulation domain resolution in z-direction
       * \param[in] output_path   The path where the output files should be written to
      */
      Continuum(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ, std::filesystem::path const& output_path) noexcept
        : NX{NX}, NY{NY}, NZ{NZ}, output_path{output_path}, p{vtkSmartPointer<vtkImageData>::New()}, 
          u{vtkSmartPointer<vtkImageData>::New()}, v{vtkSmartPointer<vtkImageData>::New()}, w{vtkSmartPointer<vtkImageData>::New()} {
        allocateScalar_(p);
        allocateScalar_(u);
        allocateScalar_(v);
        allocateScalar_(w);
        return;
      }

      /**\fn        setP
       * \brief     Set the pressure to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \param[in] value   The value the pressure should be set to
      */
      inline void setP(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept;

      /**\fn        setU
       * \brief     Set the x-velocity to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \param[in] value   The value the pressure should be set to
      */
      inline void setU(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept;

      /**\fn        setV
       * \brief     Set the y-velocity to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \param[in] value   The value the pressure should be set to
      */
      inline void setV(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept;

      /**\fn        setW
       * \brief     Set the z-velocity to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \param[in] value   The value the pressure should be set to
      */
      inline void setW(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept;

      /**\fn        getP
       * \brief     Get the pressure at the coordinates \param x, \param y and \param z
       *
       * \param[in] x   The x-coordinate of cell
       * \param[in] y   The y-coordinate of cell
       * \param[in] z   The z-coordinate of cell
       * \return    The pressure value at the coordinates \param x, \param y and \param z
      */
      inline T getP(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept;

      /**\fn        getU
       * \brief     Get the x-velocity at the coordinates \param x, \param y and \param z
       *
       * \param[in] x   The x-coordinate of cell
       * \param[in] y   The y-coordinate of cell
       * \param[in] z   The z-coordinate of cell
       * \return    The x-velocity value at the coordinates \param x, \param y and \param z
      */
      inline T getU(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept;

      /**\fn        getV
       * \brief     Get the y-velocity at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \return    The y-velocity value at the coordinates \param x, \param y and \param z
      */
      inline T getV(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept;

      /**\fn        getW
       * \brief     Get the z-velocity at the coordinates \param x, \param y and \param z
       *
       * \param[in] x   The x-coordinate of cell
       * \param[in] y   The y-coordinate of cell
       * \param[in] z   The z-coordinate of cell
       * \return    The z-velocity value at the coordinates \param x, \param y and \param z
      */
      inline T getW(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept;

      /**\fn        exportToVtk
       * \brief     Export velocity and density at current time step to a *.vtk-file that can then be
       *            read by visualisation applications like ParaView.
       * \warning   *.vtk export is comparably slow and large! Better export to meta-image *.mhd-files!
       *
       * \param[in] timestamp   The current time stamp that will be used for the name
      */
      void exportToVtk(double const timestamp) const noexcept;

      /**\fn        exportToMhd
       * \brief     Export velocity and density at current time step to a meta-image *.mhd-file that can then be
       *            read by visualisation applications like ParaView.
       *
       * \param[in] timestamp     The current time stamp that will be used for the name
       * \param[in] is_compress   Boolean variable signaling whether the output should be compressed or not
      */
      void exportToMhd(double const timestamp, bool const is_compress = true) const noexcept;

    private:
      /**\fn        allocateScalar_
       * \brief     Allocate a certain scalar with the corresponding data type
       *
       * \param[in] image_data   The meta-image data to be allocated
      */
      void allocateScalar_(vtkSmartPointer<vtkImageData>& image_data) noexcept;

      /**\fn        setImageDataComponent_
       * \brief     Set an image data component to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] image_data   The meta-image data to be accessed
       * \param[in] x            The x-coordinate of cell
       * \param[in] y            The y-coordinate of cell
       * \param[in] z            The z-coordinate of cell
       * \param[in] value        The value the pressure should be set to
      */
      inline void setImageDataComponent_(vtkSmartPointer<vtkImageData>& image_data, std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept;

      /**\fn        getImageDataComponent_
       * \brief     Get the image data component at the coordinates \param x, \param y and \param z
       *
       * \param[in] image_data   The meta-image data to be accessed
       * \param[in] x            The x-coordinate of cell
       * \param[in] y            The y-coordinate of cell
       * \param[in] z            The z-coordinate of cell
       * \return    The x-velocity value at the coordinates \param x, \param y and \param z
      */
      inline T getImageDataComponent_(vtkSmartPointer<vtkImageData> const& image_data, std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept;

      std::int32_t NX;
      std::int32_t NY;
      std::int32_t NZ;

      std::filesystem::path output_path;

      vtkSmartPointer<vtkImageData> p;
      vtkSmartPointer<vtkImageData> u;
      vtkSmartPointer<vtkImageData> v;
      vtkSmartPointer<vtkImageData> w;
  };

  template <typename T>
  void Continuum<T>::setP(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    setImageDataComponent_(p, x, y, z, value);
    return;
  }

  template <typename T>
  void Continuum<T>::setU(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    setImageDataComponent_(u, x, y, z, value);
    return;
  }

  template <typename T>
  void Continuum<T>::setV(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    setImageDataComponent_(v, x, y, z, value);
    return;
  }

  template <typename T>
  void Continuum<T>::setW(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    setImageDataComponent_(w, x, y, z, value);
    return;
  }

  template <typename T>
  T Continuum<T>::getP(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    return getImageDataComponent_(p, x, y, z);
  }

  template <typename T>
  T Continuum<T>::getU(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    return getImageDataComponent_(u, x, y, z);
  }

  template <typename T>
  T Continuum<T>::getV(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    return getImageDataComponent_(v, x, y, z);
  }

  template <typename T>
  T Continuum<T>::getW(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    return getImageDataComponent_(w, x, y, z);
  }

  template <typename T>
  void Continuum<T>::exportToVtk(double const timestamp) const noexcept {
    std::filesystem::create_directories(output_path);

    // Export all different scalars
    std::filesystem::path const filename_p {"p_" + toString(timestamp)}; 
    exportImageDataToVtk(p, output_path, filename_p);
    std::filesystem::path const filename_u {"u_" + toString(timestamp)}; 
    exportImageDataToVtk(u, output_path, filename_u);
    std::filesystem::path const filename_v {"v_" + toString(timestamp)}; 
    exportImageDataToVtk(v, output_path, filename_v);
    std::filesystem::path const filename_w {"w_" + toString(timestamp)}; 
    exportImageDataToVtk(w, output_path, filename_w);
    return;
  }

  template <typename T>
  void Continuum<T>::exportToMhd(double const timestamp, bool const is_compress) const noexcept {
    std::filesystem::create_directories(output_path);

    // Export all different scalars
    std::filesystem::path const filename_p {"p_" + toString(timestamp)}; 
    exportImageDataToMhd(p, output_path, filename_p, is_compress);
    std::filesystem::path const filename_u {"u_" + toString(timestamp)}; 
    exportImageDataToMhd(u, output_path, filename_u, is_compress);
    std::filesystem::path const filename_v {"v_" + toString(timestamp)}; 
    exportImageDataToMhd(v, output_path, filename_v, is_compress);
    std::filesystem::path const filename_w {"w_" + toString(timestamp)}; 
    exportImageDataToMhd(w, output_path, filename_w, is_compress);
    return;
  }

  template <typename T>
  void Continuum<T>::allocateScalar_(vtkSmartPointer<vtkImageData>& image_data) noexcept {
    // Set domain size to the one given by the geometry
    double const domain_size[3] = {static_cast<double>(NX), static_cast<double>(NY), static_cast<double>(NZ)};

    int const resolution[3] = {NX, NY, NZ};
    image_data->SetDimensions(resolution);
    double const spacing[3] = {domain_size[0]/resolution[0], domain_size[1]/resolution[1], domain_size[2]/resolution[2]};
    image_data->SetSpacing(spacing);

    // Set origin to the one given by the geometry
    //double const origin[3] = {bounding_box[0] + spacing[0]/2.0, bounding_box[2] + spacing[1]/2.0,  bounding_box[4] + spacing[2]/2.0};
    //image_data->SetOrigin(origin);

    if constexpr (std::is_same_v<T,float>) {
      image_data->AllocateScalars(VTK_FLOAT, 1);
    } else if constexpr (std::is_same_v<T,double>) {
      image_data->AllocateScalars(VTK_DOUBLE, 1);
    }
    image_data->GetPointData()->GetScalars()->Fill(0);

    return;
  }

  template <typename T>
  void Continuum<T>::setImageDataComponent_(vtkSmartPointer<vtkImageData>& image_data, std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept {
    if constexpr (std::is_same_v<T,float>) {
      image_data->SetScalarComponentFromFloat(x, y, z, 0, value);
    } else if constexpr (std::is_same_v<T,double>) {
      image_data->SetScalarComponentFromDouble(x, y, z, 0, value);
    } else {
      static_assert(true, "Invalid template parameter T");
    }
    return;
  }

  template <typename T>
  T Continuum<T>::getImageDataComponent_(vtkSmartPointer<vtkImageData> const& image_data, std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept {
    if constexpr (std::is_same_v<T,float>) {
      return image_data->GetScalarComponentAsFloat(x, y, z, 0);
    } else if constexpr (std::is_same_v<T,double>) {
      return image_data->GetScalarComponentAsDouble(x, y, z, 0);
    } else {
      static_assert(true, "Invalid template parameter T");
    }
  }

}

#endif // LBT_CONTINUUM
