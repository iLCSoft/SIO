
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

}
