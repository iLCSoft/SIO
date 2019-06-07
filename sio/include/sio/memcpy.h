#pragma once

// -- std headers
#include <cstring>  // std::memcpy, std::size_t

namespace sio {

  /**
   *  @brief  memcpy class
   *
   *  Perform raw data copy and deals with plateform endian-ness
   */
  class memcpy {
  public:
    // static API only
    memcpy() = delete ;

    /**
     *  @brief  Perform a reverse byte copy
     *
     *  @param  from the input bytes address to copy
     *  @param  dest the output destination of copied bytes
     *  @param  size the size of the element in the bytes
     *  @param  count the number of elements to copy
     */
    static void reverse_copy( const unsigned char *const from, unsigned char *dest, std::size_t size, std::size_t count ) ;

    /**
     *  @brief  Perform a byte array copy
     *
     *  @param  from the input bytes address to copy
     *  @param  dest the output destination of copied bytes
     *  @param  size the size of the element in the bytes
     *  @param  count the number of elements to copy
     */
    static void copy( const unsigned char *const from, unsigned char *dest, std::size_t size, std::size_t count ) ;
  };
  
}
