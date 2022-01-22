#ifndef LBT_BOUNDARY_TYPE
#define LBT_BOUNDARY_TYPE
#pragma once

/**
 * \file     type.hpp
 * \mainpage Holds enum for types of boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/


namespace lbt {
  namespace boundary {

    /**\enum  Type
     * \brief Strongly typed for different boundary condition types
    */
    enum class Type { Velocity, Pressure };

  }
}

#endif // LBT_BOUNDARY_TYPE
