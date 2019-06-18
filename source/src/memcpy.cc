// -- sio headers
#include <sio/memcpy.h>

namespace sio {

  void memcpy::reverse_copy( const sio::byte *const from, sio::byte *dest, std::size_t size, std::size_t count ) {
    for( std::size_t icnt = 0; icnt < count; icnt++ ) {
      for( std::size_t i=0 ; i<size ; i++ ) {
        dest [ i ] = from [ size - (i+1) ] ;
      }
    }
  }

  //--------------------------------------------------------------------------

  void memcpy::copy( const sio::byte *const from, sio::byte *dest, std::size_t size, std::size_t count ) {
  #ifdef SIO_BIG_ENDIAN
    std::memcpy( dest, from, size * count ) ;
  #else
    sio::memcpy::reverse_copy( from, dest, size, count ) ;
  #endif
  }

}
