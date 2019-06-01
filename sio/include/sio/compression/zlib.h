#pragma once

// -- sio headers
#include <sio/definitions.h>

// -- zlib headers
#include <zlib.h>

namespace sio {
  
  class zlib_compression {
  public:
    struct compress_opts {
      
    };
    
    struct uncompress_opts {
      ///< The length of the input buffer to uncompress
      std::size_t        _inlen {0} ;
      ///< An optional shift from the input buffer start position
      std::size_t        _inshift {0} ;
      ///< The expected output buffer length after uncompression
      std::size_t        _outlen {0} ;
    };

  public:
    /// Default constructor
    zlib_compression() = default ;
    /// Default destructor
    ~zlib_compression() = default ;

    /**
     *  @brief  Uncompress the buffer and return a new buffer
     * 
     *  @param  inbuf the input buffer to uncompress
     *  @param  opts the uncompression options
     */
    buffer uncompress( const buffer &inbuf, const uncompress_opts &opts ) ;
  };
  
  
  buffer zlib_compression::uncompress( const buffer &inbuf, const uncompress_opts &opts ) {
    if( not inbuf.valid() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Buffer is not valid" ) ;
    }
    if( opts._inshift + opts._inlen > inbuf.size() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Invalid uncompression options: incompatible sizes" ) ;
    }
    auto indata = inbuf.ptr( opts._inshift ) ;
    auto insize = opts._inlen ;
    auto outsize = opts._outlen ;
    // setup the zstream interface
    z_stream zstream ;
    zstream.zalloc = Z_NULL ;
    zstream.zfree  = Z_NULL ;
    zstream.opaque = 0 ;
    auto zstat = inflateInit( &zstream ) ;
    if( Z_OK != zstat ) {
      SIO_THROW( sio::error_code::compress_error, "Zlib inflate init failed with status " + std::to_string( zstat ) ) ;
    }
    buffer outbuf( outsize ) ;
    zstream.next_in   = outbuf.data() ;
    zstream.avail_in  = outsize ;
    zstream.total_in  = 0 ;
    zstream.next_out  = const_cast<byte *>(indata) ; // thank you zlib for your awesome constness handling ...
    zstream.avail_out = insize ;
    zstream.total_out = 0 ;
    try {
      zstat = inflate( &zstream, Z_FINISH ) ;
      if( Z_OK != zstat ) {
        SIO_THROW( sio::error_code::compress_error, "Zlib inflate failed with status " + std::to_string( zstat ) ) ;
      }
    }
    catch( sio::exception &e ) {
      // inflateEnd after a failure
      zstat = inflateEnd( &zstream ) ;
      if( Z_OK != zstat ) {
        SIO_THROW( sio::error_code::compress_error, "Zlib inflateEnd failed with status " + std::to_string( zstat ) ) ;
      }
      throw e ;
    }
    // inflateEnd at end of function
    zstat = inflateEnd( &zstream ) ;
    if( Z_OK != zstat ) {
      SIO_THROW( sio::error_code::compress_error, "Zlib inflateEnd failed with status " + std::to_string( zstat ) ) ;
    }
    return outbuf ;
  }
}

