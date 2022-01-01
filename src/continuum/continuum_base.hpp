#ifndef LBT_CONTINUUM_BASE
#define LBT_CONTINUUM_BASE

/**
 * \file     continuum_base.hpp
 * \mainpage Base class for continuum properties
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <filesystem>


namespace lbt {

  /**\class  ContinuumBase
   * \brief  Base class for the macroscopic variables
   *
   * \tparam T   Floating data type used for simulation
  */
  template <typename T>
  class ContinuumBase {
    public:
      /**\fn        setP
       * \brief     Set the pressure to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \param[in] value   The value the pressure should be set to
      */
      virtual void setP(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept = 0;

      /**\fn        setU
       * \brief     Set the x-velocity to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \param[in] value   The value the pressure should be set to
      */
      virtual void setU(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept = 0;

      /**\fn        setV
       * \brief     Set the y-velocity to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \param[in] value   The value the pressure should be set to
      */
      virtual void setV(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept = 0;

      /**\fn        setW
       * \brief     Set the z-velocity to a given value \param value at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \param[in] value   The value the pressure should be set to
      */
      virtual void setW(std::int32_t const x, std::int32_t const y, std::int32_t const z, T const value) noexcept = 0;

      /**\fn        getP
       * \brief     Get the pressure at the coordinates \param x, \param y and \param z
       *
       * \param[in] x   The x-coordinate of cell
       * \param[in] y   The y-coordinate of cell
       * \param[in] z   The z-coordinate of cell
       * \return    The pressure value at the coordinates \param x, \param y and \param z
      */
      virtual T getP(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept = 0;

      /**\fn        getU
       * \brief     Get the x-velocity at the coordinates \param x, \param y and \param z
       *
       * \param[in] x   The x-coordinate of cell
       * \param[in] y   The y-coordinate of cell
       * \param[in] z   The z-coordinate of cell
       * \return    The x-velocity value at the coordinates \param x, \param y and \param z
      */
      virtual T getU(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept = 0;

      /**\fn        getV
       * \brief     Get the y-velocity at the coordinates \param x, \param y and \param z
       *
       * \param[in] x       The x-coordinate of cell
       * \param[in] y       The y-coordinate of cell
       * \param[in] z       The z-coordinate of cell
       * \return    The y-velocity value at the coordinates \param x, \param y and \param z
      */
      virtual T getV(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept = 0;

      /**\fn        getW
       * \brief     Get the z-velocity at the coordinates \param x, \param y and \param z
       *
       * \param[in] x   The x-coordinate of cell
       * \param[in] y   The y-coordinate of cell
       * \param[in] z   The z-coordinate of cell
       * \return    The z-velocity value at the coordinates \param x, \param y and \param z
      */
      virtual T getW(std::int32_t const x, std::int32_t const y, std::int32_t const z) const noexcept = 0;

      /**\fn    save
       * \brief Function for exporting the domain to a file
       * 
       * \param[in] timestamp   The current timestamp
      */
      virtual void save(double const timestamp) const noexcept = 0;

    protected:
      /**\fn    ContinuumBase
       * \brief Class constructor
       * 
       * \param[in] NX            Simulation domain resolution in x-direction
       * \param[in] NY            Simulation domain resolution in y-direction
       * \param[in] NZ            Simulation domain resolution in z-direction
       * \param[in] output_path   The path where the output files should be written to
      */
      ContinuumBase(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ, std::filesystem::path const& output_path) noexcept
        : NX{NX}, NY{NY}, NZ{NZ}, output_path{output_path} {
        return;
      }

      ContinuumBase() = delete;
      ContinuumBase(ContinuumBase&) = default;
      ContinuumBase& operator = (ContinuumBase&) = default;
      ContinuumBase(ContinuumBase&&) = default;
      ContinuumBase& operator = (ContinuumBase&&) = default;

      std::int32_t NX;
      std::int32_t NY;
      std::int32_t NZ;
      std::filesystem::path output_path;
  };

}

#endif // LBT_CONTINUUM_BASE
