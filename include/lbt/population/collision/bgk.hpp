/**
 * \file     bgk.hpp
 * \mainpage Class for BGK collision operator
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_COLLISION_BGK
#define LBT_COLLISION_BGK
#pragma once


namespace lbt {
  namespace collision {

    /**\class  Bgk
     * \brief  Class that holds macroscopic values with indexing based on the A-A access pattern
     *
     * \tparam LT   Static lattice::DdQq class containing discretisation parameters
     * \tparam NP   Number of populations stored side by side in a single merged grid (default = 1)
    */
    template <class LT>
    class Bgk {
      public:
    };

  }

}

#endif // LBT_COLLISION_BGK
