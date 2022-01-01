#ifndef LBT_CONTINUUM
#define LBT_CONTINUUM

/**
 * \file     continuum.hpp
 * \mainpage Class for continuum properties based
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include "vtk_continuum.hpp"
// Potentially switch between different implementations


namespace lbt {

  // Convenient alias for continuum
  template <typename T>
  using Continuum = VtkContinuum<T>;

}

#endif // LBT_CONTINUUM
