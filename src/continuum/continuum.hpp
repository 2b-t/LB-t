#ifndef LBT_CONTINUUM
#define LBT_CONTINUUM
#pragma once

/**
 * \file     continuum.hpp
 * \mainpage Class for continuum properties based
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include "../general/use_vtk.hpp"

#ifdef LBT_USE_VTK
  #include "vtk_continuum.hpp"

  namespace lbt {
    template <typename T>
    using Continuum = VtkContinuum<T>;
  }
#else
  #include "simple_continuum.hpp"

  namespace lbt {
    template <typename T>
    using Continuum = SimpleContinuum<T>;
  }
#endif // LBT_USE_VTK

#endif // LBT_CONTINUUM
