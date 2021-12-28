#ifndef LBT_UNITS
#define LBT_UNITS
#pragma once

/**\file     units.hpp
 * \mainpage Contains definitions for different physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iostream>


namespace lbt {
  namespace unit {
    namespace detail {
      /**\class UnitBase
       * \brief Base class for all physical units
       *        Uses a special form of the Curiously Recurring Template Pattern (CRTP) for
       *        not having to re-declare the different operators between the same physical units
       * \tparam The child class inheriting from UnitBase
      */
      template <typename T>
      class UnitBase {
        public:
          /// All the operators for the derived class

          /**\fn    operator+
           * \brief Addition operator between two derived unit classes of the same type
           * 
           * \param[in] a   One derived unit class
           * \param[in] b   Another derived unit class
           * \return The sum of the two units
          */
          friend constexpr T operator+ (T const& a, T const& b) noexcept {
            return T{a.value + b.value};
          }

          /**\fn    operator-
           * \brief Subtraction operator between two derived unit classes of the same type
           * 
           * \param[in] a   One derived unit class
           * \param[in] b   Another derived unit class
           * \return The difference between the two units
          */
          friend constexpr T operator- (T const& a, T const& b) noexcept {
            return T{a.value - b.value};
          }

          /**\fn    operator*
           * \brief Multiplication operator between a constant and a derived class
           * 
           * \param[in] c   A floating number constant
           * \param[in] a   One derived unit class
           * \return The derived class scaled by the constant
          */
          friend constexpr T operator* (long double const c, T const& t) noexcept {
            return T{c*t.value};
          }

          /**\fn    operator/
           * \brief Division operator between a derived class and a constant
           * 
           * \param[in] t   One derived unit class
           * \param[in] c   A floating number constant
           * \return The derived class scaled by the constant
          */
          friend constexpr T operator/ (T const& t, long double const c) noexcept {
            return T{t.value/c};
          }

          /**\fn    operator<<
           * \brief Output stream operator between an output stream and the derived unit class
           * 
           * \param[in] out    The output stream
           * \param[in] t      One derived unit class
           * \return The output stream containing the outputted unit class
          */
          friend std::ostream& operator<< (std::ostream& os, T const& t) noexcept {
            os << t.value;
            return os;
          }

          /**\fn    set
           * \brief Setter for value stored inside
           * 
           * \param[in] Value stored inside the unit class given in its base unit
          */
          constexpr void set(long double const val) noexcept {
            value = val;
            return;
          }

          /**\fn    get
           * \brief Getter for value stored inside
           * 
           * \return Value stored inside the unit class in its base unit
          */
          constexpr auto get() const noexcept {
            return value;
          }

        protected:
          /**\fn    UnitBase
           * \brief Constructor of base class for all physical units
           * 
           * \param[in] value   The value to be stored inside the class
          */
          constexpr UnitBase(long double const value) noexcept 
            : value{value} {
            return;
          }
          UnitBase(UnitBase const&) = default;
          UnitBase& operator= (UnitBase const&) = default;
          UnitBase(UnitBase&&) = default;
          UnitBase& operator= (UnitBase&&) = default;

          template <typename U> friend class UnitBase; // Currently unused, maybe useful in the future

          long double value; // Stored unit value
      };
    }

    /**\class Length
     * \brief Unit class for lengths and distances
    */
    class Length : public lbt::unit::detail::UnitBase<Length> {
      public:
        /**\fn    Length
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit meters
        */
        constexpr Length(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Length(Length const&) = default;
        Length& operator= (Length const&) = default;
        Length(Length&&) = default;
        Length& operator= (Length&&) = default;
    };
    using Distance = Length;

    /**\class Time
     * \brief Unit class for time and duration
    */
    class Time : public lbt::unit::detail::UnitBase<Time> {
      public:
        /**\fn    Time
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit seconds
        */
        constexpr Time(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Time(Time const&) = default;
        Time& operator= (Time const&) = default;
        Time(Time&&) = default;
        Time& operator= (Time&&) = default;
    };
    using Duration = Time;

    /**\class Mass
     * \brief Unit class for mass
    */
    class Mass : public lbt::unit::detail::UnitBase<Mass> {
      public:
        /**\fn    Mass
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit kilograms
        */
        constexpr Mass(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Mass(Mass const&) = default;
        Mass& operator= (Mass const&) = default;
        Mass(Mass&&) = default;
        Mass& operator= (Mass&&) = default;
    };

    /**\class Area
     * \brief Unit class for areas
    */
    class Area : public lbt::unit::detail::UnitBase<Area> {
      public:
        /**\fn    Area
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit square meters
        */
        constexpr Area(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Area(Area const&) = default;
        Area& operator= (Area const&) = default;
        Area(Area&&) = default;
        Area& operator= (Area&&) = default;
    };

    /**\class Volume
     * \brief Unit class for three-dimensional volume in the base unit cubic meters
    */
    class Volume : public lbt::unit::detail::UnitBase<Volume> {
      public:
        /**\fn    Volume
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class
        */
        constexpr Volume(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Volume(Volume const&) = default;
        Volume& operator= (Volume const&) = default;
        Volume(Volume&&) = default;
        Volume& operator= (Volume&&) = default;
    };

    /**\class Velocity
     * \brief Unit class for velocity
    */
    class Velocity : public lbt::unit::detail::UnitBase<Velocity> {
      public:
        /**\fn    Velocity
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit meters per seconds
        */
        constexpr Velocity(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Velocity(Velocity const&) = default;
        Velocity& operator= (Velocity const&) = default;
        Velocity(Velocity&&) = default;
        Velocity& operator= (Velocity&&) = default;
    };

    /**\class Density
     * \brief Unit class for density
    */
    class Density : public lbt::unit::detail::UnitBase<Density> {
      public:
        /**\fn    Density
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit kilograms per cubic meter
        */
        constexpr Density(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Density(Density const&) = default;
        Density& operator= (Density const&) = default;
        Density(Density&&) = default;
        Density& operator= (Density&&) = default;
    };

    /**\class KinematicViscosity
     * \brief Unit class for fluid kinematic viscosity
    */
    class KinematicViscosity : public lbt::unit::detail::UnitBase<KinematicViscosity> {
      public:
        /**\fn    KinematicViscosity
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit meter squared per second
        */
        constexpr KinematicViscosity(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        KinematicViscosity(KinematicViscosity const&) = default;
        KinematicViscosity& operator= (KinematicViscosity const&) = default;
        KinematicViscosity(KinematicViscosity&&) = default;
        KinematicViscosity& operator= (KinematicViscosity&&) = default;
    };

    /// Conversion operators for mixed units

    /**\fn        operator*
     * \brief     Multiplication operator between two lengths resulting in an area
     * 
     * \param[in] a   A length
     * \param[in] b   Another length
     * \return    The area resulting from the product of two lengths
    */
    constexpr Area operator* (Length const& a, Length const& b) noexcept {
      return Area{a.get()*b.get()};
    }

    /**\fn        operator*
     * \brief     Multiplication operator between a length and an area resulting in an volume
     * 
     * \param[in] l   A length
     * \param[in] a   An area
     * \return    The volume resulting from the product of a length and an area
    */
    constexpr Volume operator* (Length const& l, Area const& a) noexcept {
      return Volume{a.get()*l.get()};
    }

    /**\fn        operator*
     * \brief     Multiplication operator between a length and an area resulting in an volume
     *            For symmetry reasons
     * 
     * \param[in] a   An area
     * \param[in] l   A length
     * \return    The volume resulting from the product of a length and an area
    */
    constexpr Volume operator* (Area const& a, Length const& l) noexcept {
      return Volume{a.get()*l.get()};
    }

    /**\fn        operator/
     * \brief     Division operator between a length and a time resulting in a velocity
     * 
     * \param[in] l   A distance
     * \param[in] t   The time it took to travel the distance
     * \return    The velocity resulting from the division of the length by the time
    */
    constexpr Velocity operator/ (Length const& l, Time const& t) noexcept {
      return Velocity{l.get()/t.get()};
    }

    /**\fn        operator/
     * \brief     Division operator between a mass and a volume resulting in a density
     * 
     * \param[in] m   A mass
     * \param[in] v   The volume that the mass takes up
     * \return    The density resulting from the division of the mass by the time
    */
    constexpr Density operator/ (Mass const& m, Volume const& v) noexcept {
      return Density{m.get()/v.get()};
    }

    /**\fn        operator/
     * \brief     Division operator between an area and a time resulting in a kinematic viscosity
     * 
     * \param[in] a   An area
     * \param[in] t   A time
     * \return    The kinematic viscosity resulting from the division of an area by a time
    */
    constexpr KinematicViscosity operator/ (Area const& a, Time const& t) noexcept {
      return KinematicViscosity{a.get()/t.get()};
    }
  }

}

#endif // LBT_UNITS
