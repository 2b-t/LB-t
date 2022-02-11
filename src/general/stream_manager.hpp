/**
 * \file     stream_manager.hpp
 * \mainpage Utilities for directing an output stream to a file or turning them off
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_STREAM_MANAGER
#define LBT_STREAM_MANAGER
#pragma once

#include <ostream>


namespace lbt {

  /**\class   Stream
   * \brief   Class for redirecting an output stream to another output stream such as a file
   *          or to deactivate it
  */
  class StreamManager {
    public:
      /**\fn        StreamManager
       * \brief     Constructor redirects the output stream \p src to \p dest
       *            Source: https://www.cplusplus.com/reference/ios/ios/rdbuf/
       *            Usage: std::ofstream log_file {"log_file.txt"};
       *                   lbt::StreamRedirector stream_redirector {std::cout, log_file};
       *
       * \param[in] src   The source output stream
      */
      explicit StreamManager(std::ostream& src) noexcept 
        : src_stream{src}, src_buf{src.rdbuf()} {
        return;
      }
      StreamManager() = delete;
      StreamManager(StreamManager const&) = delete;
      StreamManager(StreamManager&&) = delete;
      StreamManager& operator=(StreamManager const&) = delete;
      StreamManager& operator=(StreamManager&&) = delete;

      /**\fn        redirect
       * \brief     Redirect the output stream src_stream to \p dest
       *            Source: https://www.cplusplus.com/reference/ios/ios/rdbuf/
       *
       * \param[in] dst   The destination output stream
      */
      void redirect(std::ostream& dst) noexcept {
        src_stream.rdbuf(dst.rdbuf());
        return;
      }

      /**\fn     restore
       * \brief  Restore the src_stream to the initial stream buffer
       *         Source: https://www.cplusplus.com/reference/ios/ios/rdbuf/
       *
       * \return Boolean variable signaling success or failure
      */
      bool restore() noexcept {
        bool is_success {false};
        if (src_buf != nullptr) {
          src_stream.rdbuf(src_buf);
        }
        return is_success;
      }

      /**\fn    turnOn
       * \brief Turn on the filestream
       *        Source: https://en.cppreference.com/w/cpp/io/ios_base/iostate
      */
      void turnOn() noexcept {
        src_stream.clear();
        return;
      }

      /**\fn    turnOff
       * \brief Turn off the filestream
       *        Source: https://stackoverflow.com/a/30185095
      */
      void turnOff() noexcept {
        src_stream.setstate(std::ios_base::failbit);
        return;
      }

      /**\fn    ~StreamRedirector
       * \brief Destructor restores the redirection of the output stream
      */
      ~StreamManager() noexcept {
        restore();
        turnOn();
        return;
      }

    protected:
      std::ostream& src_stream; ///< Reference to the source stream
      std::streambuf* src_buf;  ///< Stream buffer for restoring
  };

}

#endif // LBT_STREAM_MANAGER
