
// -- sio headers
#include <sio/helpers.h>

namespace sio {
  
  inline block::block( const std::string &nam, sio::version_type vers ) :
    _version(vers),
    _name(nam) {
    if( not sio::string_helper::validate( _name ) ) {
      SIO_THROW( sio::error_code::invalid_argument, "Block name '" + name + "' is invalid!" ) ;
    }
  }
  
  //--------------------------------------------------------------------------
  
  inline const std::string &block::name() const noexcept {
    return _name ;
  }

  //--------------------------------------------------------------------------

  inline sio::version_type block::version() const noexcept {
    return _version ;
  }
  
}