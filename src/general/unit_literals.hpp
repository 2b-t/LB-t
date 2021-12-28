#ifndef LBT_UNIT_LITERALS
#define LBT_UNIT_LITERALS
#pragma once

/**\file     unit_literals.hpp
 * \mainpage Contains definitions for different physical units and their literals
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iostream>


namespace lbt {
  namespace unit {
    namespace detail {
      // Special form of Curiousy Recurring Template Pattern (CRTP)
      template <typename T>
      class UnitBase {
        public:
          constexpr UnitBase(long double const value) noexcept 
            : value{value} {
            return;
          }
          // Default others

          // Trick for not having to rewrite all operators in each derived class
          friend constexpr T operator+ (T const& a, T const& b) noexcept {
            return T{a.value + b.value};
          }
          friend constexpr T operator- (T const& a, T const& b) noexcept {
            return T{a.value - b.value};
          }
          friend constexpr T operator* (long double const c, T const& a) noexcept {
            return T{c*a.value};
          }
          friend constexpr T operator/ (T const& a, long double const c) noexcept {
            return T{a.value/c};
          }
          friend std::ostream& operator<< (std::ostream& out, T const& unit) noexcept {
            out << unit.value;
            return out;
          }
          constexpr auto getValue() const noexcept {
            return value;
          }
        protected:
          // Maybe useful in the future
          template <typename U> friend class UnitBase;

          long double value;
      };
    }

    class Length : public lbt::unit::detail::UnitBase<Length> {
      public:
        using lbt::unit::detail::UnitBase<Length>::UnitBase;
        // Explicitely default others
    };

    class Time : public lbt::unit::detail::UnitBase<Time> {
      public:
        using lbt::unit::detail::UnitBase<Time>::UnitBase;
    };

    class Mass : public lbt::unit::detail::UnitBase<Mass> {
      public:
        using lbt::unit::detail::UnitBase<Mass>::UnitBase;
    };

    class Area : public lbt::unit::detail::UnitBase<Area> {
      public:
        using lbt::unit::detail::UnitBase<Area>::UnitBase;
    };

    class Volume : public lbt::unit::detail::UnitBase<Volume> {
      public:
        using lbt::unit::detail::UnitBase<Volume>::UnitBase;
    };

    class Velocity : public lbt::unit::detail::UnitBase<Velocity> {
      public:
        using lbt::unit::detail::UnitBase<Velocity>::UnitBase;
    };

    class Density : public lbt::unit::detail::UnitBase<Density> {
      public:
        using lbt::unit::detail::UnitBase<Density>::UnitBase;
    };

    class KinematicViscosity : public lbt::unit::detail::UnitBase<KinematicViscosity> {
      public:
        using lbt::unit::detail::UnitBase<KinematicViscosity>::UnitBase;
    };

    // Conversion operators for mixed units
    constexpr Area operator* (Length const& a, Length const& b) noexcept {
      return Area{a.getValue()*b.getValue()};
    }
    constexpr Volume operator* (Length const& l, Area const& a) noexcept {
      return Volume{a.getValue()*l.getValue()};
    }
    constexpr Volume operator* (Area const& a, Length const& l) noexcept {
      return Volume{a.getValue()*l.getValue()};
    }

    constexpr Velocity operator/ (Length const& d, Time const& t) noexcept {
      return Velocity{d.getValue()/t.getValue()};
    }

    constexpr Density operator/ (Mass const& m, Volume const& v) noexcept {
      return Density{m.getValue()/v.getValue()};
    }

    constexpr KinematicViscosity operator/ (Area const& a, Time const& t) noexcept {
      return KinematicViscosity{a.getValue()/t.getValue()};
    }
  }

  namespace literals {
    constexpr lbt::unit::Length operator "" _km(long double const k) noexcept {
      return lbt::unit::Length{1000.0*k};
    }
    constexpr lbt::unit::Length operator "" _m(long double const m) noexcept {
      return lbt::unit::Length{m};
    }
    constexpr lbt::unit::Length operator "" _dm(long double const d) noexcept {
      return lbt::unit::Length{d/10.0};
    }
    constexpr lbt::unit::Length operator "" _cm(long double const c) noexcept {
      return lbt::unit::Length{c/100.0};
    }
    constexpr lbt::unit::Length operator "" _mm(long double const m) noexcept {
      return lbt::unit::Length{m/1000.0};
    }
    constexpr lbt::unit::Length operator "" _um(long double const u) noexcept {
      return lbt::unit::Length{u/1.0e6};
    }

    constexpr lbt::unit::Time operator "" _h(long double const h) noexcept {
      return lbt::unit::Time{3600.0*h};
    }
    constexpr lbt::unit::Time operator "" _min(long double const m) noexcept {
      return lbt::unit::Time{60.0*m};
    }
    constexpr lbt::unit::Time operator "" _s(long double const s) noexcept {
      return lbt::unit::Time{s};
    }
    constexpr lbt::unit::Time operator "" _ms(long double const m) noexcept {
      return lbt::unit::Time{m/1000.0};
    }
    constexpr lbt::unit::Time operator "" _us(long double const u) noexcept {
      return lbt::unit::Time{u/1.0e6};
    }

    constexpr lbt::unit::Mass operator "" _t(long double const t) noexcept {
      return lbt::unit::Mass{1000.0*t};
    }
    constexpr lbt::unit::Mass operator "" _kg(long double const k) noexcept {
      return lbt::unit::Mass{k};
    }
    constexpr lbt::unit::Mass operator "" _g(long double const g) noexcept {
      return lbt::unit::Mass{g/1000.0};
    }

    constexpr lbt::unit::Area operator "" _m2(long double const m) noexcept {
      return lbt::unit::Area{m};
    }
    constexpr lbt::unit::Area operator "" _cm2(long double const c) noexcept {
      return c*lbt::unit::Area{1.0_cm*1.0_cm};
    }
    constexpr lbt::unit::Area operator "" _mm2(long double const m) noexcept {
      return m*lbt::unit::Area{1.0_mm*1.0_mm};
    }

    constexpr lbt::unit::Volume operator "" _m3(long double const m) noexcept {
      return lbt::unit::Volume{m};
    }
    constexpr lbt::unit::Volume operator "" _cm3(long double const c) noexcept {
      return c*lbt::unit::Volume{1.0_cm*1.0_cm*1.0_cm};
    }
    constexpr lbt::unit::Volume operator "" _mm3(long double const m) noexcept {
      return m*lbt::unit::Volume{1.0_mm*1.0_mm*1.0_mm};
    }

    // Poise
    constexpr lbt::unit::KinematicViscosity operator "" _P(long double const p) noexcept {
      return lbt::unit::KinematicViscosity{0.1*p};
    }
    // Centipoise
    constexpr lbt::unit::KinematicViscosity operator "" _cP(long double const c) noexcept {
      return lbt::unit::KinematicViscosity{c/1.0e3};
    }
    // Stokes
    constexpr lbt::unit::KinematicViscosity operator "" _St(long double const s) noexcept {
      return lbt::unit::KinematicViscosity{s/1.0e4};
    }
    // Centistokes
    constexpr lbt::unit::KinematicViscosity operator "" _cSt(long double const c) noexcept {
      return lbt::unit::KinematicViscosity{c/1.0e6};
    }
  }
}

#endif // LBT_UNIT_LITERALS
