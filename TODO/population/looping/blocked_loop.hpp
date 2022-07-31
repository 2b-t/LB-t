#ifndef LBT_LOOP
#define LBT_LOOP

/**
 * \file     loop.hpp
 * \mainpage Loop accessing operator
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cmath>
#include <cstdint>
#if __has_include (<omp.h>)
  #include <omp.h>
#endif

#include "../general/constexpr_math.hpp"
#include "../general/type_definitions.hpp"


namespace lbt {

  /**\class  Loop
   * \brief  Loop pattern
   *
   * \tparam CO   Collision operator
   * \tparam C    Continuum
   * \tparam P    Population
  */
  template <template <typename C, typename P> class CO, typename C, typename P>
  class Loop: public CO<C,P> {
    public:
      Loop() = delete;
      Loop& operator = (Loop&) = delete;
      Loop(Loop&&) = delete;
      Loop& operator = (Loop&&) = delete;
      Loop(Loop const&) = delete;

      void collideAndStream(bool const is_save) noexcept;

    protected:
      /// parallelism: 3D blocks
      //  each cell gets a block of cells instead of a single cell
      static constexpr std::int32_t   BLOCK_SIZE_ = 32; ///< loop block size
      static constexpr std::int32_t NUM_BLOCKS_Z_ = cem::ceil(static_cast<double>(NZ) / BLOCK_SIZE_); ///< number of blocks in each dimension
      static constexpr std::int32_t NUM_BLOCKS_Y_ = cem::ceil(static_cast<double>(NY) / BLOCK_SIZE_);
      static constexpr std::int32_t NUM_BLOCKS_X_ = cem::ceil(static_cast<double>(NX) / BLOCK_SIZE_);
      static constexpr std::int32_t   NUM_BLOCKS_ = NUM_BLOCKS_X_*NUM_BLOCKS_Y_*NUM_BLOCKS_Z_; ///< total number of blocks
  };

  template <template <typename C, typename P> class CO, typename C, typename P> template <bool E>
  void Loop<CO,C,P>::collideAndStream(bool const is_save) noexcept {
    #pragma omp parallel for
    for (std::int64_t block = 0; block < NUM_BLOCKS_; ++block) {
      std::int64_t const z_start = BLOCK_SIZE_ * (block / (NUM_BLOCKS_X_*NUM_BLOCKS_Y_));
      std::int64_t const   z_end = std::min(z_start + BLOCK_SIZE_, NZ);

      for (std::int64_t z = z_start; z < z_end; ++z) {
        std::int64_t const y_start = BLOCK_SIZE_*((block % (NUM_BLOCKS_X_*NUM_BLOCKS_Y_)) / NUM_BLOCKS_X_);
        std::int64_t const   y_end = std::min(y_start + BLOCK_SIZE_, NY);

        for (std::int64_t y = y_start; y < y_end; ++y) {
          std::int64_t const x_start = BLOCK_SIZE_*(block % NUM_BLOCKS_X_);
          std::int64_t const   x_end = std::min(x_start + BLOCK_SIZE_, NX);

          for (std::int64_t x = x_start; x < x_end; ++x) {
            CO<C,P>::template collideAndStream<E>(x,y,z,is_save);
          }
        }
      }
    }
    return;
  }
}

#endif // LBT_LOOP
