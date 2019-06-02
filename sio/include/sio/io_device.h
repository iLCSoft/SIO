#pragma once

// -- sio headers
#include <sio/buffer.h>

namespace sio {

  class read_device {
  public:
    using cursor_type = std::size_t ;
    using size_type = std::size_t ;

  public:
    read_device() = default ;
    read_device( const read_device & ) = default ;
    read_device( read_device && ) = default ;
    read_device& operator=( const read_device & ) = default ;
    read_device& operator=( read_device && ) = default ;
    ~read_device() = default ;

    void set_buffer( buffer_span &&buf ) ;
    void set_buffer( const buffer_span &buf ) ;

    cursor_type position() const ;
    void seek( cursor_type pos ) ;

    template <typename T>
    void read( T &var ) ;

    template <typename T>
    void read( T &var, size_type count ) ;

  private:
    ///< The buffer span
    buffer_span         _buffer {} ;
    ///< The device cursor
    cursor_type         _cursor {0} ;
  };

}

#include <sio/details/io_device_impl.h>
