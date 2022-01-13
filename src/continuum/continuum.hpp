#ifndef LBT_CONTINUUM
#define LBT_CONTINUUM
#pragma once

/**
 * \file     continuum.hpp
 * \mainpage Class for continuum properties based
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#if __has_include (<vtkSmartPointer.h>)
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
#endif

#endif // LBT_CONTINUUM
