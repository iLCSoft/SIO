#pragma once

// -- sio headers
#include <sio/buffer.h>

namespace sio {
  
  template <typename T>
  template <typename ...Args>
  inline compression<T>::compression( Args &&...args ) :
    _impl(args...) {
    /* nop */
  }
  
  //--------------------------------------------------------------------------
  
  template <typename T>
  inline buffer compression<T>::uncompress( const buffer &inbuf, const uncompress_opts &opts ) {
    byte_array bytes ;
    _impl.uncompress( inbuf, bytes, opts ) ;
    return buffer( std::move( bytes ) ) ;
  }
  
}







