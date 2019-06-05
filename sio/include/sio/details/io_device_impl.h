
#include <sio/api.h>

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
  inline void read_device::data( T &var ) {
    data( &var, 1 ) ;
  }

  //--------------------------------------------------------------------------

  template <typename T>
  inline void read_device::data( T *var, size_type count ) {
    _cursor += sio::api::read( _buffer, var, _cursor, count ) ;
  }
  
  //--------------------------------------------------------------------------
  
  void read_device::pointer_to( ptr_type *ptr ) {
    // Read.  Keep a record of the "match" quantity read from the buffer and
    // the location in memory which will need relocating.
    // Placeholder value for 'pointer to'
    unsigned int match = 0 ;
    data( match ) ;
    // Ignore match = 0x00000000.  This is basically a null pointer which can
    // never be relocated, so don't fill the multimap with a lot of useless
    // information.
    if( match != 0x00000000 ) {
      sio::pointer_to_map::value_type entry = { reinterpret_cast<void *>(match), ptr } ;
      _pointer_to.insert( entry ) ;
    }
    *ptr = static_cast<sio::ptr_type>( match ) ;
  }
  
  //--------------------------------------------------------------------------
  
  void read_device::pointed_at( ptr_type *ptr ) {
    // Read.  Keep a record of the "match" quantity read from the buffer and
    // the location in memory which will need relocating.
    unsigned int match = 0 ;
    data( match ) ;
    // Ignore match = SIO_ptag. This is basically a pointer target which was
    // never relocated when the record was written. i.e. nothing points to it!
    // Don't clutter the maps with information that can never be used. 
    if( match != 0xffffffff ) {
      pointed_at_map::value_type entry = { reinterpret_cast<void *>( match ), ptr } ;
      _pointed_at.insert( entry ) ;
    }
  }
  
  //--------------------------------------------------------------------------
  
  void read_device::pointer_relocation() {
    sio::api::read_relocation( _pointed_at, _pointer_to ) ;
    _pointer_to.clear() ;
    _pointed_at.clear() ;
  }

}
