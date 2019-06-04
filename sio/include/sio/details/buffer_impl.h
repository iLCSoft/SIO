#pragma once

// -- std headers
#include <sstream>

// -- sio headers
#include <sio/exception.h>
#include <sio/helpers.h>

namespace sio {

  inline buffer_span::buffer_span() :
    _isnull(true) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  inline buffer_span::buffer_span( const container &bytes ) :
    _first( bytes.begin() ),
    _last( bytes.end() ) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  inline buffer_span::buffer_span( const_iterator first, const_iterator last ) :
    _first(first),
    _last(last) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  inline buffer_span::buffer_span( const_iterator first, std::size_t count ) :
    _first(first),
    _last(std::next(first, count)) {
    /* nop */
  }

  //--------------------------------------------------------------------------

  inline buffer_span::const_iterator buffer_span::begin() const {
    return _first ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span::const_iterator buffer_span::end() const {
    return _last ;
  }

  //--------------------------------------------------------------------------

  inline const buffer_span::element_type *buffer_span::data() const {
    return _isnull ? nullptr : &(*_first) ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span::const_reference buffer_span::front() const {
    return (*_first) ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span::const_reference buffer_span::back() const {
    return *(_last-1) ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span::const_reference buffer_span::operator[]( index_type index ) const {
    return data() [ index ] ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span::const_reference buffer_span::at( index_type index ) const {
    if( index >= size() ) {
      std::stringstream ss ;
      ss << "index: " << index << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return data() [ index ] ;
  }
  
  //--------------------------------------------------------------------------
  
  buffer_span::const_pointer buffer_span::ptr( index_type index ) const {
    return data() + index ;
  }

  //--------------------------------------------------------------------------

  inline std::size_t buffer_span::size() const {
    return _isnull ? 0 : std::distance( _first, _last ) ;
  }

  //--------------------------------------------------------------------------

  inline bool buffer_span::empty() const {
    return (size() == 0) ;
  }

  //--------------------------------------------------------------------------

  inline bool buffer_span::valid() const {
    return (not _isnull) ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span::operator bool() const noexcept {
    return valid() ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span buffer_span::subspan( index_type start ) const {
    if( start >= size() ) {
      std::stringstream ss ;
      ss << "start: " << start << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return buffer_span( std::next(_first, start) , _last ) ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span buffer_span::subspan( index_type start, std::size_t count ) const {
    if( start+count >= size() ) {
      std::stringstream ss ;
      ss << "start: " << start << ", count: " << count << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return buffer_span( std::next(_first, start) , std::next(_first, start+count) ) ;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  inline buffer::buffer( size_type len ) :
    _bytes( len, sio::null_byte ) {
    if( 0 == len ) {
      SIO_THROW( sio::error_code::invalid_argument, "Can't construct a buffer with length of 0!" ) ;
    }
  }

  //--------------------------------------------------------------------------

  inline buffer::buffer( container &&bytes ) :
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

  inline buffer::const_iterator buffer::begin() const {
    return _bytes.begin() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_iterator buffer::end() const {
    return _bytes.end() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::iterator buffer::begin() {
    return _bytes.begin() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::iterator buffer::end() {
    return _bytes.end() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_reverse_iterator buffer::rbegin() const {
    return _bytes.rbegin() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_reverse_iterator buffer::rend() const {
    return _bytes.rend() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::reverse_iterator buffer::rbegin() {
    return _bytes.rbegin() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::reverse_iterator buffer::rend() {
    return _bytes.rend() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_reference buffer::operator[]( index_type index ) const {
    return _bytes [ index ] ;
  }

  //--------------------------------------------------------------------------

  inline buffer::reference buffer::operator[]( index_type index ) {
    return _bytes [ index ] ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_reference buffer::at( index_type index ) const {
    return _bytes.at( index ) ;
  }

  //--------------------------------------------------------------------------

  inline buffer::reference buffer::at( index_type index ) {
    return _bytes.at( index ) ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_reference buffer::front() const {
    return _bytes.front() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::reference buffer::front() {
    return _bytes.front() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_reference buffer::back() const {
    return _bytes.back() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::reference buffer::back() {
    return _bytes.back() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_pointer buffer::data() const {
    return _bytes.data() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::pointer buffer::data() {
    return _bytes.data() ;
  }

  //--------------------------------------------------------------------------

  inline buffer::const_pointer buffer::ptr( index_type index ) const {
    return &(_bytes[ index ]) ;
  }

  //--------------------------------------------------------------------------

  inline buffer::pointer buffer::ptr( index_type index ) {
    return &(_bytes[ index ]) ;
  }

  //--------------------------------------------------------------------------

  inline buffer::size_type buffer::size() const {
    return _bytes.size() ;
  }

  //--------------------------------------------------------------------------

  inline bool buffer::empty() const {
    return _bytes.empty() ;
  }

  //--------------------------------------------------------------------------

  inline void buffer::resize( size_type newsize ) {
    _bytes.resize( newsize ) ;
  }

  //--------------------------------------------------------------------------

  inline buffer::size_type buffer::expand( std::size_t nbytes ) {
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

  inline buffer buffer::reuse() {
    buffer new_buffer( std::move( _bytes ) ) ;
    _valid = false ;
    return new_buffer ;
  }

  //--------------------------------------------------------------------------

  inline bool buffer::valid() const {
    return _valid ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span buffer::span() const {
    return buffer_span( begin() , end() ) ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span buffer::span( index_type start ) const {
    if( start >= size() ) {
      std::stringstream ss ;
      ss << "start: " << start << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return buffer_span( std::next(begin(), start) , end() ) ;
  }

  //--------------------------------------------------------------------------

  inline buffer_span buffer::span( index_type start, size_type count ) const {
    if( start+count >= size() ) {
      std::stringstream ss ;
      ss << "start: " << start << ", count: " << count << ", size: " << size() ;
      SIO_THROW( error_code::out_of_range, ss.str() ) ;
    }
    return buffer_span( std::next(begin(), start) , std::next(begin(), start+count) ) ;
  }

}
