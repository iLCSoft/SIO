#pragma once

namespace sio {
  
  inline buffer_view::buffer_view( const buffer &buf ) :
    _buffer(buf) {
    /* nop */
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte &buffer_view::operator[]( index_type index ) const {
    return _buffer [ index ] ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::const_iterator buffer_view::begin() const {
    return _buffer.begin() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::const_iterator buffer_view::end() const {
    return _buffer.end() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::const_reverse_iterator buffer_view::rbegin() const {
    return _buffer.rbegin() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::const_reverse_iterator buffer_view::rend() const {
    return _buffer.rend() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte &buffer_view::front() const {
    return _buffer.front() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte &buffer_view::back() const {
    return _buffer.back() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline std::size_t buffer_view::size() const {
    return _buffer.size() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte &buffer_view::at( index_type index ) const {
    return _buffer.at( index ) ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte *buffer_view::data() const {
    return _buffer.data() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline bool buffer_view::empty() const {
    return _buffer.empty() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte *buffer_view::ptr( index_type index ) const {
    return _buffer.ptr( index ) ;
  }
  
  //--------------------------------------------------------------------------
  
  inline bool buffer_view::valid() const {
    return _buffer.valid() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline std::size_t buffer_view::read( byte *ptr, index_type position, std::size_t length, std::size_t count ) const {
    return _buffer.read( ptr, position, length, count ) ;
  }
  
  //--------------------------------------------------------------------------
  
  template <typename T>
  inline std::size_t buffer_view::read( T *ptr, index_type position, std::size_t count ) const {
    return _buffer.read<T>( ptr, position, count ) ;
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
    
  inline buffer::buffer( std::size_t len ) :
    _bytes( len, sio::null_byte ) {
    if( 0 == len ) {
      SIO_THROW( sio::error_code::invalid_argument, "Can't construct a buffer with length of 0!" ) ;
    }
  }
  
  //--------------------------------------------------------------------------

  inline buffer::buffer( byte_array &&bytes ) :
    _bytes( std::move( bytes ) ) {
    /* nop */
  }
  
  //--------------------------------------------------------------------------
  
  inline buffer::buffer( buffer&& rhs ) {
    _bytes = std::move( rhs._bytes ) ;
    _valid = rhs._valid ;
    rhs._valid = false ;
  }

  //--------------------------------------------------------------------------

  inline buffer& buffer::operator=( buffer&& rhs ) {
    _bytes = std::move( rhs._bytes ) ;
    _valid = rhs._valid ;
    rhs._valid = false ;
    return *this ;
  }
  
  //--------------------------------------------------------------------------

  inline byte_array::const_iterator buffer::begin() const {
    return _bytes.begin() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::const_iterator buffer::end() const {
    return _bytes.end() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::iterator buffer::begin() {
    return _bytes.begin() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::iterator buffer::end() {
    return _bytes.end() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::const_reverse_iterator buffer::rbegin() const {
    return _bytes.rbegin() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::const_reverse_iterator buffer::rend() const {
    return _bytes.rend() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::reverse_iterator buffer::rbegin() {
    return _bytes.rbegin() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte_array::reverse_iterator buffer::rend() {
    return _bytes.rend() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte &buffer::operator[]( index_type index ) const {
    return _bytes [ index ] ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte &buffer::operator[]( index_type index ) {
    return _bytes [ index ] ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte &buffer::at( index_type index ) const {
    return _bytes.at( index ) ;
  }
  
  //--------------------------------------------------------------------------

  inline byte &buffer::at( index_type index ) {
    return _bytes.at( index ) ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte &buffer::front() const {
    return _bytes.front() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte &buffer::front() {
    return _bytes.front() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte &buffer::back() const {
    return _bytes.back() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte &buffer::back() {
    return _bytes.back() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte *buffer::data() const {
    return _bytes.data() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte *buffer::data() {
    return _bytes.data() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline const byte *buffer::ptr( index_type index ) const {
    return &(_bytes[ index ]) ;
  }
  
  //--------------------------------------------------------------------------
  
  inline byte *buffer::ptr( index_type index ) {
    return &(_bytes[ index ]) ;
  }
  
  //--------------------------------------------------------------------------
  
  inline std::size_t buffer::size() const {
    return _bytes.size() ;
  }
  
  //--------------------------------------------------------------------------
  
  inline bool buffer::empty() const {
    return _bytes.empty() ;
  }
  
  //--------------------------------------------------------------------------

  inline void buffer::resize( std::size_t newsize ) {
    return _bytes.resize( newsize ) ;
  }
  
  //--------------------------------------------------------------------------
  
  inline std::size_t buffer::expand( std::size_t nbytes ) {
    auto len = size() ;
    resize( len + nbytes ) ;
    return len ;
  }
  
  //--------------------------------------------------------------------------

  inline void buffer::clear( bool shrink ) {
    _bytes.clear() ;
    if( shrink ) {
      _bytes.shrink_to_fit() ;
    }
  }
  
  //--------------------------------------------------------------------------

  inline std::size_t buffer::write( const byte *const addr, index_type position, std::size_t length, std::size_t count ) {
    if( not valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    const auto bytelen = length*count ;
    const auto padlen = (bytelen + sio::padding) & sio::padding_mask ;
    if( position + padlen >= size() ) {
      expand( padlen ) ;
    }
    auto ptr_write = ptr( position ) ;
    sio::memcpy_helper::copy( addr, ptr_write, length, count ) ;
    for( auto bytcnt = bytelen; bytcnt < padlen; bytcnt++ ) {
      *(ptr_write + bytcnt) = sio::null_byte ;
    }
    return padlen ;
  }
  
  //--------------------------------------------------------------------------

  template <typename T>
  inline std::size_t buffer::write( const T *const ptr, index_type position, std::size_t count ) {
    return write( SIO_CBYTE_CAST(ptr), position, sizeof(T), count ) ;
  }
  
  //--------------------------------------------------------------------------

  inline std::size_t buffer::read( byte *addr, index_type position, std::size_t length, std::size_t count ) const {
    if( not valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    const auto bytelen = length*count ;
    const auto padlen = (bytelen + sio::padding) & sio::padding_mask ;
    if( position + padlen >= size() ) {
      SIO_THROW( sio::error_code::invalid_argument, "Can't read " + std::to_string(padlen) + " bytes out of buffer (pos=" + std::to_string(position) + ")" ) ;
    }
    auto ptr_read = ptr( position ) ;
    sio::memcpy_helper::copy( ptr_read, addr, length, count ) ;
    return padlen ;
  }
  
  //--------------------------------------------------------------------------

  template <typename T>
  inline std::size_t buffer::read( T *ptr, index_type position, std::size_t count ) const {
    return read( SIO_BYTE_CAST(ptr), position, sizeof(T), count ) ;
  }
  
  //--------------------------------------------------------------------------

  inline buffer buffer::reuse() {
    buffer new_buffer( std::move( _bytes ) ) ;
    _valid = false ;
    return new_buffer ;
  }
  
  //--------------------------------------------------------------------------
  
  inline bool buffer::valid() const {
    return _valid ;
  }
  
}

