// -- sio headers
#include <sio/memcpy.h>
#include <sio/definitions.h>

namespace sio {
  
  void memcpy::reverse_copy( const unsigned char *const from, unsigned char *dest, std::size_t size, std::size_t count ) {
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

  void memcpy::copy( const unsigned char *const from, unsigned char *dest, std::size_t size, std::size_t count ) {
  #ifdef SIO_BIG_ENDIAN
    std::memcpy( dest, from, size * count ) ;
  #else
    sio::memcpy::reverse_copy( from, dest, size, count ) ;
  #endif
  }
  
}