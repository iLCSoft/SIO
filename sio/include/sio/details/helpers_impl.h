
namespace sio {
  
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
