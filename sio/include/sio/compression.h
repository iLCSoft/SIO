#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {
  
  class buffer_span ;
  class buffer ;
  
  class compression {
  public:
    // static API only
    compression() = delete ;

    /**
     *  @brief  Uncompress the buffer and return a new buffer
     *
     *  @param  compressor the compression implementation
     *  @param  inbuf the input buffer to uncompress
     *  @param  args the uncompression options (implementation dependant)
     */
    template <typename compT, typename ...Args>
    static buffer uncompress( compT &compressor, const buffer_span &inbuf, Args ...args ) ;
    
    /**
     *  @brief  Compress the buffer and return a new buffer
     *
     *  @param  compressor the compression implementation
     *  @param  inbuf the input buffer to compress
     *  @param  args the compression options (implementation dependant)
     */
    template <typename compT, typename ...Args>
    static buffer compress( compT &compressor, const buffer_span &inbuf, Args ...args ) ;
  };
    
}

#include <sio/details/compression_impl.h>
