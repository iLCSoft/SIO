
#include <sio/helpers.h>

namespace sio {
  
  
  inline read_device::read_device( buffer_span buf ) :
    _buffer(std::move(buf)) {
    /* nop */
  }
  
  //--------------------------------------------------------------------------

  inline void read_device::set_buffer( const buffer_span &buf ) {
    _buffer = buf ;
  }

  //--------------------------------------------------------------------------

  inline void read_device::set_buffer( buffer_span &&buf ) {
    _buffer = buf ;
  }

  //--------------------------------------------------------------------------

  inline read_device::cursor_type read_device::position() const {
    return _cursor ;
  }

  //--------------------------------------------------------------------------

  inline void read_device::seek( cursor_type pos ) {
    _cursor = pos ;
  }

  //--------------------------------------------------------------------------

  template <typename T>
  inline void read_device::read( T &var ) {
    read( &var, 1 ) ;
  }

  //--------------------------------------------------------------------------

  template <typename T>
  inline void read_device::read( T *var, size_type count ) {
    _cursor += sio::io_helper::read( _buffer, var, _cursor, count ) ;
  }

}
