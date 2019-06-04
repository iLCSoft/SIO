
#include <sio/exception.h>
#include <sio/buffer.h>
#include <sio/io_device.h>

namespace sio {
  
  inline bool compression_helper::is_compressed( options_type opts ) {
    return static_cast<bool>( opts & sio::compression_bit ) ;
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  inline version_helper::version_type version_helper::encode_version( major_type major, minor_type minor ) noexcept {
    return (((major) << 16) + (minor)) ;
  }

  //--------------------------------------------------------------------------

  inline version_helper::minor_type version_helper::minor_version( version_type version ) noexcept {
    return ((version) & 0x0000ffff) ;
  }

  //--------------------------------------------------------------------------

  inline version_helper::major_type version_helper::major_version( version_type version ) noexcept {
    return (((version) & 0xffff0000) >> 16) ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  void memcpy_helper::reverse_copy( const unsigned char *const from, unsigned char *dest, std::size_t size, std::size_t count ) {
    dest += size;
    std::size_t jump = size << 1;
    const unsigned char *lfrom = from ;
    for( std::size_t icnt = 0; icnt < count; icnt++ ) {
      for( std::size_t ibyt = 0; ibyt < size; ibyt++  ) {
        *--dest = *lfrom++;
      }
      dest += jump;
    }
  }

  //--------------------------------------------------------------------------

  void memcpy_helper::copy( const unsigned char *const from, unsigned char *dest, std::size_t size, std::size_t count ) {
#ifdef SIO_BIG_ENDIAN
    SIO_DEBUG( "Big endian copy" ) ;
    memcpy( dest, from, size * count ) ;
#else
    SIO_DEBUG( "Little endian copy" ) ;
    memcpy_helper::reverse_copy( from, dest, size, count ) ;
#endif
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  inline bool string_helper::validate( const std::string &name ) {
    auto cname = name.c_str() ;
    if( *cname < 0 ) {
      return false;
    }
    if( !isalpha( (int)*cname ) && *cname != '_' ) {
      return false;
    }
    for( cname += 1; *cname != '\0'; cname++ ) {
      if( *cname < 0 ) {
        return false;
      }
      if( !isalnum( (int)*cname ) && *cname != '_' ) {
        return false;
      }
    }
    return true;
  }

  //--------------------------------------------------------------------------

  inline bool string_helper::valid_record_name( const std::string &name ) {
    if( not string_helper::validate( name ) ) {
      return false ;
    }
    if( name.size() > sio::max_record_name_len ) {
      return false ;
    }
    return true ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  template <class bufT, typename T>
  inline typename bufT::size_type io_helper::read( const bufT &buffer, T *ptr, typename bufT::index_type position, typename bufT::size_type count ) {
    return io_helper::read( buffer, SIO_BYTE_CAST(ptr), sizeof(T), position, count ) ;
  }

  //--------------------------------------------------------------------------

  template <class bufT>
  inline typename bufT::size_type io_helper::read( const bufT &buffer, typename bufT::pointer ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) {
    if( not buffer.valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    const auto bytelen = length*count ;
    const auto padlen = (bytelen + sio::padding) & sio::padding_mask ;
    SIO_DEBUG( "Reading... len: " << length << ", count: " << count << ", padlen: " << padlen ) ;
    if( position + padlen > buffer.size() ) {
      std::stringstream ss ;
      ss << "Can't read " << padlen << " bytes out of buffer (pos=" << position << ")" ;
      SIO_THROW( sio::error_code::invalid_argument, ss.str() ) ;
    }
    auto ptr_read = buffer.ptr( position ) ;
    sio::memcpy_helper::copy( SIO_CUCHAR_CAST(ptr_read), SIO_UCHAR_CAST(ptr), length, count ) ;
    return padlen ;
  }

  //--------------------------------------------------------------------------

  template <class bufT, typename T>
  inline typename bufT::size_type io_helper::write( bufT &buffer, const T *const ptr, typename bufT::index_type position, typename bufT::size_type count ) {
    return io_helper::write( buffer, SIO_CBYTE_CAST(ptr), sizeof(T), position, count ) ;
  }

  //--------------------------------------------------------------------------

  template <class bufT>
  inline typename bufT::size_type io_helper::write( bufT &buffer, typename bufT::const_pointer const ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) {
    if( not buffer.valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    const auto bytelen = length*count ;
    const auto padlen = (bytelen + sio::padding) & sio::padding_mask ;
    if( position + padlen >= buffer.size() ) {
      buffer.expand( padlen ) ;
    }
    auto ptr_write = buffer.ptr( position ) ;
    sio::memcpy_helper::copy( SIO_CUCHAR_CAST(ptr), SIO_UCHAR_CAST(ptr_write), length, count ) ;
    for( auto bytcnt = bytelen; bytcnt < padlen; bytcnt++ ) {
      *(ptr_write + bytcnt) = sio::null_byte ;
    }
    return padlen ;
  }
  
  
  // inline record_info io_helper::read_record_info( sio::ifstream &stream, bool rewind ) {
  // 
  // }
  
  //--------------------------------------------------------------------------
  
  inline std::pair<record_info, buffer> io_helper::read_record( sio::ifstream &stream ) {
    if( not stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "ifstream is not open!" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state!" ) ;
    }
    record_info info ;
    info._file_start = stream.tellg() ;
    buffer read_buffer( 256 ) ;
    stream.read( read_buffer.data(), 8 ) ;
    SIO_DEBUG( "Read " << stream.gcount() << " bytes out of file" ) ;
    if( stream.eof() ) {
      SIO_THROW( sio::error_code::eof, "Reached end of file !" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after reading first record bytes!" ) ;
    }
    unsigned int buftyp(0) ;
    read_device device( read_buffer.span() ) ;
    device.read( info._header_length ) ;
    SIO_DEBUG( "Read " << info._header_length << " from buffer" ) ;
    device.read( buftyp ) ;
    SIO_DEBUG( "Read " << buftyp << " from buffer" ) ;
    if( buftyp != sio::record_marker ) {
      stream.setstate( sio::ifstream::failbit ) ;
      SIO_THROW( sio::error_code::no_marker, "Record marker not found!" ) ;
    }
    // Interpret: 3) The options word.
    //            4) The length of the record data (compressed).
    //            5) The length of the record name (uncompressed).
    //            6) The length of the record name.
    //            7) The record name.
    stream.read( read_buffer.data(), info._header_length-8 ) ;
    SIO_DEBUG( "Read " << stream.gcount() << " bytes out of file" ) ;
    device.seek( 0 ) ;
    unsigned int name_length(0);
    device.read( info._options ) ;
    device.read( info._data_length ) ;
    device.read( info._uncompressed_length ) ;
    device.read( name_length ) ;
    info._name.assign( name_length, '\0' ) ;
    device.read( &(info._name[0]), name_length ) ;
    // a bit of debugging ...
    SIO_DEBUG( "=== Read record info ====" ) ;
    SIO_DEBUG( "  options:     " << info._options ) ;
    SIO_DEBUG( "  data_length: " << info._data_length ) ;
    SIO_DEBUG( "  ucmp_length: " << info._uncompressed_length ) ;
    SIO_DEBUG( "  name_length: " << name_length ) ;
    SIO_DEBUG( "  record_name: " << info._name ) ;
    // resize the record buffer and read out the full record buffer
    read_buffer.resize( info._header_length + info._data_length ) ;
    stream.read( read_buffer.ptr(info._header_length), info._data_length ) ;
    SIO_DEBUG( "Read " << stream.gcount() << " bytes out of file" ) ;
    const auto compressed = sio::compression_helper::is_compressed( info._options ) ;
    // if the record is compressed skip the read pointer over 
    // any padding bytes that may have been inserted to make 
    // the next record header start on a four byte boundary in the file.
    if( compressed ) {
      auto pad_length = (4 - (info._data_length & sio::bit_align)) & sio::bit_align ;
      if( pad_length ) {
        if( not stream.seekg( pad_length, std::ios_base::cur ).good() ) {
          SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state!" ) ;
        }
      }
    }
    info._file_end = stream.tellg() ;
    return std::make_pair(info, std::move(read_buffer)) ;
  }

}
