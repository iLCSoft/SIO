#pragma once

// -- sio headers
#include <sio/buffer.h>

namespace sio {
  
  template <typename compT, typename ...Args>
  inline buffer compression::uncompress( compT &compressor, const buffer_span &inbuf, Args ...args ) {
    return compressor.uncompress( inbuf, args... ) ;
  }
  
  //--------------------------------------------------------------------------
  
  template <typename compT, typename ...Args>
  inline buffer compression::compress( compT &compressor, const buffer_span &inbuf, Args ...args ) {
    return compressor.compress( inbuf, args... ) ;
  }
  
}







