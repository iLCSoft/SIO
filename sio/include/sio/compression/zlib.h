#pragma once

// -- sio headers
#include <sio/definitions.h>

// -- zlib headers
#include <zlib.h>

namespace sio {
  
  class zlib_compression {
  public:
    /// Default constructor
    zlib_compression() = default ;
    /// Default destructor
    ~zlib_compression() = default ;

    /**
     *  @brief  Uncompress the buffer and return a new buffer
     * 
     *  @param  inbuf the input buffer to uncompress
     *  @param  out_len the uncompressed buffer length (read out from the record header)
     */
    buffer uncompress( const buffer_span &inbuf, std::size_t out_len ) ;
    
    /**
     *  @brief  Compress the buffer and return a new buffer
     * 
     *  @param  inbuf the input buffer to compress
     *  @param  comp_level the compression level
     */
    buffer compress( const buffer_span &inbuf, int comp_level = Z_DEFAULT_COMPRESSION ) ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  buffer zlib_compression::uncompress( const buffer_span &inbuf, std::size_t out_len ) {
    if( not inbuf.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Buffer is not valid" ) ;
    }
    // zlib uncompress requires to pass an address of uLongf type ...
    uLongf outsize = out_len ;
    buffer outbuf( out_len ) ;
    auto zstat = ::uncompress( (Bytef*)outbuf.data(), &outsize, (const Bytef*)inbuf.data(), inbuf.size() ) ;
    if( Z_OK != zstat ) {
      std::stringstream ss ;
      ss << "Zlib uncompression failed with status " << zstat ;
      SIO_THROW( sio::error_code::compress_error, ss.str() ) ;
    }
    return outbuf ;
  }
  
  //--------------------------------------------------------------------------
  
  buffer zlib_compression::compress( const buffer_span &inbuf, int comp_level ) {
    if( not inbuf.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Buffer is not valid" ) ;
    }
    if( (comp_level < -1) or (comp_level > 9) ) {
      SIO_THROW( sio::error_code::invalid_argument, "Invalid compression level" ) ;
    }
    // comp_bound is a first estimate of the compressed size.
    // After compression, the real output size is returned,
    // this is why the buffer is resized after calling compress2().
    auto comp_bound = ::compressBound( inbuf.size() ) ;
    buffer outbuf( comp_bound ) ;
    auto zstat = ::compress2( (Bytef*)outbuf.data(), &comp_bound, (const Bytef*)inbuf.data(), inbuf.size(), comp_level ) ;
    if( Z_OK != zstat ) {
      std::stringstream ss ;
      ss << "Zlib compression failed with status " << zstat ;
      SIO_THROW( sio::error_code::compress_error, ss.str() ) ;
    }
    outbuf.resize( comp_bound ) ;
    return outbuf ;
  }
}

