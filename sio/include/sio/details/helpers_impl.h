
#include <sio/exception.h>

namespace sio {

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

  void memcpy_helper::reverse_copy( const byte *const from, byte *dest, std::size_t size, std::size_t count ) {
    dest += size;
    std::size_t jump = size << 1;
    const byte *lfrom = from ;
    for( std::size_t icnt = 0; icnt < count; icnt++ ) {
      for( std::size_t ibyt = 0; ibyt < size; ibyt++  ) {
        *--dest = *lfrom++;
      }
      dest += jump;
    }
  }

  //--------------------------------------------------------------------------

  void memcpy_helper::copy( const byte *const from, byte *dest, std::size_t size, std::size_t count ) {
#ifdef SIO_BIG_ENDIAN
    memcpy( dest, from, size * count ) ;
#else
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
    return io_helper::read( buffer, SIO_BYTE_CAST(ptr), position, sizeof(T), count ) ;
  }

  //--------------------------------------------------------------------------

  template <class bufT>
  inline typename bufT::size_type io_helper::read( const bufT &buffer, typename bufT::pointer ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) {
    if( not buffer.valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    const auto bytelen = length*count ;
    const auto padlen = (bytelen + sio::padding) & sio::padding_mask ;
    if( position + padlen >= buffer.size() ) {
      std::stringstream ss ;
      ss << "Can't read " << padlen << " bytes out of buffer (pos=" << position << ")" ;
      SIO_THROW( sio::error_code::invalid_argument, ss.str() ) ;
    }
    auto ptr_read = buffer.ptr( position ) ;
    sio::memcpy_helper::copy( ptr_read, ptr, length, count ) ;
    return padlen ;
  }

  //--------------------------------------------------------------------------

  template <class bufT, typename T>
  inline typename bufT::size_type io_helper::write( bufT &buffer, const T *const ptr, typename bufT::index_type position, typename bufT::size_type count ) {
    return io_helper::write( buffer, SIO_CBYTE_CAST(ptr), position, sizeof(T), count ) ;
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
    sio::memcpy_helper::copy( ptr, ptr_write, length, count ) ;
    for( auto bytcnt = bytelen; bytcnt < padlen; bytcnt++ ) {
      *(ptr_write + bytcnt) = sio::null_byte ;
    }
    return padlen ;
  }

}
