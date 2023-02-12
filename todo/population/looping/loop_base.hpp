#ifndef LBT_LOOP_BASE
#define LBT_LOOP_BASE

/**
 * \file     loop_base.hpp
 * \mainpage Loop accessing operator
 * \author   Tobit Flatscher (github.com/2b-t)
*/



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

      template <bool E>
      void collideAndStream(bool const is_save) noexcept;
  };

}

#endif // LBT_LOOP_BASE
