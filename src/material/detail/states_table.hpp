/**
 * \file     states_table.hpp
 * \mainpage Contains methods for interpolating physical properties of real gases based on tables with known thermodynamic state
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_STATES_TABLE
#define LBT_MATERIAL_STATES_TABLE
#pragma once

#include <array>
#include <limits>
#include <tuple>

#include "../../constexpr_math/constexpr_math.hpp"
#include "../../unit/units.hpp"
#include "state.hpp"


namespace lbt {
  namespace material {

    // In order to allow constexpr values prior to C++20 we will use a fixed-sized array for storing the values to interpolate the desired state from
    template <std::size_t N>
    using States = std::array<State, N>;

    /**\class  StatesTable
     * \brief  Class for calculating the thermodynamic state of a fluid via interpolation from a set of given points as an alternative to an analytical
     *         thermal equation of state
     *         This template will be have to be specialised for each material M with N values to interpolate the desired thermodynamic state from
     *         Alternative to an analytical thermal equation of state
     *
     * \tparam M   The material of interest (removed due to GCC bug)
     * \tparam N   The number of given states that should be used for interpolation
    */
    // template <typename M, std::size_t N> /// Can't be done with specialisation due to GCC bug
    template <std::size_t N>
    class StatesTable {
      public:
        /**\fn     equationOfState
         * \brief  Method for calculating the third thermodynamic state variable from the other two via interpolation
         *
         * \tparam Z   The type of the missing thermodynamic state variable
         * \tparam X   The type of one given thermodynamic state variable
         * \tparam Y   The type of another given thermodynamic state variable
         * \return The interpolated missing thermodynamic state variable for the given material
        */
        template <typename Z, typename X, typename Y>
        static constexpr Z equationOfState(X const x, Y const y) noexcept = delete;
        /// Has to be done without specialisation due to GCC non-conformity
        //  See: https://stackoverflow.com/questions/50638053/constexpr-static-data-member-without-initializer

        /*static constexpr lbt::unit::Density equationOfState(lbt::unit::Pressure const p, lbt::unit::Temperature const t) noexcept {
          return interpolate<lbt::unit::Density>(p, t, UniformMaterial::states_);
        }

        static constexpr lbt::unit::Pressure equationOfState(lbt::unit::Temperature const t, lbt::unit::Density const d) noexcept {
          return interpolate<lbt::unit::Pressure>(t, d, UniformMaterial::states_);
        }

        static constexpr lbt::unit::Temperature equationOfState(lbt::unit::Pressure const p, lbt::unit::Density const d) noexcept {
          return interpolate<lbt::unit::Temperature>(p, d, UniformMaterial::states_);
        }*/

      protected:
        /**\fn     interpolate
         * \brief  Method for calculating the third thermodynamic state variable from the other two via interpolation
         *         This approach will only work if the types X, Y and Z are all different and correspond to Pressure, Temperature and Density!
         *
         * \tparam Z   The type of the missing thermodynamic state variable
         * \tparam X   The type of one given thermodynamic state variable
         * \tparam Y   The type of another given thermodynamic state variable
         * \return The interpolated missing thermodynamic state variable for the given material
        */
        template <typename Z, typename X, typename Y>
        static constexpr Z interpolate(X const x, Y const y, States<N> const& states) noexcept {
          /**\class  InterpolationPoint
           * \brief  Class for a state that should be used for interpolation and its distance from the desired state
           *         Only used internally inside this function
          */
          struct InterpolationPoint {
            public:
              explicit constexpr InterpolationPoint(X const x_, Y const y_, Z const z_, long double const distance_) noexcept
              : x{x_}, y{y_}, z{z_}, distance{distance_} {
                return;
              }
              InterpolationPoint() = delete;
              InterpolationPoint(InterpolationPoint const&) = default;
              InterpolationPoint& operator= (InterpolationPoint const&) = default;
              InterpolationPoint(InterpolationPoint&&) = default;
              InterpolationPoint& operator= (InterpolationPoint&&) = default;
            
              X x;
              Y y;
              Z z;
              long double distance; // The distance from the desired state
          };
        
          constexpr X x_nan {std::numeric_limits<long double>::quiet_NaN()};
          constexpr Y y_nan {std::numeric_limits<long double>::quiet_NaN()};
          constexpr Z z_nan {std::numeric_limits<long double>::quiet_NaN()};
          constexpr long double inf {std::numeric_limits<long double>::infinity()};
          InterpolationPoint xn_yn {x_nan, y_nan, z_nan, inf};
          InterpolationPoint xn_yp {x_nan, y_nan, z_nan, inf};
          InterpolationPoint xp_yn {x_nan, y_nan, z_nan, inf};
          InterpolationPoint xp_yp {x_nan, y_nan, z_nan, inf};

          // Find the four closest points to the desired state
          for (auto const& state: states) {
            auto const xs {state.template get<X>()};
            auto const ys {state.template get<Y>()};
            auto const zs {state.template get<Z>()};
            X const dx {x - xs};
            Y const dy {y - ys};
            long double const distance {lbt::cem::sqrt(lbt::cem::ipow(dx.get(), 2) + lbt::cem::ipow(dy.get(), 2))};
            if (lbt::cem::isNearlyEqualEpsRel(x.get(), xs.get()) && lbt::cem::isNearlyEqualEpsRel(y.get(), ys.get())) {
              return zs;
            } else if ((dx.get() > 0.0L) && (dy.get() > 0.0L) && (distance < xn_yn.distance)) {
              xn_yn = InterpolationPoint(xs, ys, zs, distance);
            } else if ((dx.get() > 0.0L) && (dy.get() < 0.0L) && (distance < xn_yp.distance)) {
              xn_yp = InterpolationPoint(xs, ys, zs, distance);
            } else if ((dx.get() < 0.0L) && (dy.get() > 0.0L) && (distance < xp_yn.distance)) {
              xp_yn = InterpolationPoint(xs, ys, zs, distance);
            } else if ((dx.get() < 0.0L) && (dy.get() < 0.0L) && (distance < xp_yp.distance)) {
              xp_yp = InterpolationPoint(xs, ys, zs, distance);
            }
          }
          // Perform bi-linear interpolation on resulting quadrilateral: See https://stackoverflow.com/a/23921432/9938686
          long double const a {(-xn_yn.x + xp_yn.x).get()};
          long double const b {(-xn_yn.x + xn_yp.x).get()};
          long double const c {(xn_yn.x - xn_yp.x - xp_yn.x + xp_yp.x).get()};
          long double const d {(x - xn_yn.x).get()};
          long double const e {(-xn_yn.y + xp_yn.y).get()};
          long double const f {(-xn_yn.y + xn_yp.y).get()};
          long double const g {(xn_yn.y - xn_yp.y - xp_yn.y + xp_yp.y).get()};
          long double const h {(y - xn_yn.y).get()};
          // Degenerate cases (e.g. rectangle)
          long double alpha {(x - xn_yn.x).get()/a};
          long double beta {(y - xn_yn.y).get()/f};
          // Non-degenerate cases
          if (!lbt::cem::isNearlyEqualEpsRel(c*e, - a*g) && !lbt::cem::isNearlyEqualEpsRel(c*f, - b*g)) {
            long double const i {lbt::cem::sqrt(-4.0L*(c*e - a*g)*(d*f - b*h) + lbt::cem::ipow(b*e - a*f + d*g - c*h, 2))};
            alpha = -(b*e - a*f + d*g - c*h + i)/(2.0L*c*e - 2.0L*a*g);
            beta = (b*e - a*f - d*g + c*h + i)/(2.0L*c*f - 2.0L*b*g);
            if ((alpha < 0.0L) || (alpha > 1.0L) || (beta < 0.0L) || (beta > 1.0L)) {
              alpha = (-b*e + a*f - d*g + c*h + i)/(2.0L*c*e - 2.0L*a*g);
              beta = -(-b*e + a*f + d*g - c*h + i)/(2.0L*c*f - 2.0L*b*g);
            }
          }
          Z const z {(1.0L - alpha)*((1.0L - beta)*xn_yn.z + beta*xn_yp.z) + alpha*((1.0L - beta)*xp_yn.z + beta*xp_yp.z)};
          /*auto const& p_1 = xn_yn;
          auto const& p_2 = xn_yp;
          auto const& p_3 = xp_yn;
          auto const& p_4 = xp_yp;

          auto const a_x {p_2.x - p_1.x};
          auto const a_y {p_2.y - p_1.y};
          auto const b_x {p_3.x - p_1.x};
          auto const b_y {p_3.y - p_1.y};
          auto const c_x {p_4.x - p_2.x};
          auto const c_y {p_4.y - p_2.y};

          // We exclude any degenerations to a triangle or a line and assume that the points are correct
          auto alpha {(x - p_1.x)/b_x};
          auto beta {(y - p_1.y - alpha*b_y)/(alpha*(c_y - b_y) + a_y)};

          if (!lbt::cem::isNearlyEqualEpsRel((a_x + alpha*c_x).get(), (x - p_1.x).get())) {
            auto const d {-c_x.get()*b_y.get() + b_x.get()*c_y.get()};
            auto const e {-b_x.get()*y.get() + b_x.get()*p_1.y.get() - a_x.get()*b_y.get() + c_x.get()*y.get() - c_x.get()*p_1.y.get() + 
                           x.get()*b_y.get() - p_1.x.get()*b_y.get() + b_x.get()*a_y.get() - x.get()*c_y.get() + p_1.x.get()*c_y.get()};
            auto const f {a_x.get()*y.get() - a_x.get()*p_1.y.get() - x.get()*a_y.get() + p_1.x.get()*a_y.get()};

            if (lbt::cem::isNearlyEqualEpsAbs(d, 0.0L)) {
              // Special case of an axis-aligned quadrilateral
              alpha = -f/e;
            } else {
              // Solve quadratic equation in alpha
              auto const g {lbt::cem::sqrt(lbt::cem::ipow(e, 2) - 4.0L*d*f)};
              alpha = (-e + g)/(2.0L*d);
              if ((alpha > 1) || (alpha < 0)) {
                alpha = (-e - g)/(2.0L*d);
              }
            }

            beta = (x - p_1.x - alpha*b_x)/(-alpha*b_x + a_x + alpha*c_x);
          }

          auto const z {p_1.z + alpha*(p_3.z - p_1.z) + beta*(-alpha*(p_3.z - p_1.z) + (p_2.z - p_1.z) + alpha*(p_4.z - p_2.z))};*/
          return z;
        }

        StatesTable() = default;
        StatesTable(StatesTable const&) = default;
        StatesTable& operator= (StatesTable const&) = default;
        StatesTable(StatesTable&&) = default;
        StatesTable& operator= (StatesTable&&) = default;

        /// static constexpr States<N> states_;
        //  Table containing given states to interpolate between, needs to be specialised for every material M!
    };

  }
}

#endif // LBT_MATERIAL_STATES_TABLE
