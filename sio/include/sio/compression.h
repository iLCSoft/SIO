#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {
  
  class buffer ;
  class buffer_view ;
      
  class compression_helper {
  public:
    static inline bool is_compressed( options_type opts ) {
      return static_cast<bool>( opts & sio::compression_bit ) ;
    }
  };
  
  
  template <typename T>
  class compression {
  public:
    using impl_type = T ;
    using compress_opts = typename impl_type::compress_opts ;
    using uncompress_opts = typename impl_type::uncompress_opts ;
  
  public:
    /// Default destructor
    ~compression() = default ;
    
    /**
     *  @brief  Constructor
     * 
     *  @param  args optional arguments to pass to the underlying implementation
     */
    template <typename ...Args>
    compression( Args &&...args ) ;

    /**
     *  @brief  Uncompress the buffer and return a new buffer
     * 
     *  @param  inbuf the input buffer to uncompress
     *  @param  opts the uncompression options
     */
    buffer uncompress( const buffer &inbuf, const uncompress_opts &opts ) ;
    
  private:
    ///< The compression real implementation
    impl_type              _impl ;
  };
    
}

#include <sio/details/compression_impl.h>
