#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {

  class buffer_span {
  public:
    using container = byte_array ;
    using element_type = byte_array::value_type ;
    using const_iterator = container::const_iterator ;
    using const_reference = container::const_reference ;
    using index_type = std::size_t ;

  public:
    buffer_span( const buffer_span& ) = default ;
    buffer_span( buffer_span&& ) = default ;
    ~buffer_span() = default ;
    buffer_span& operator=( const buffer_span& ) = default ;
    buffer_span& operator=( buffer_span&& ) = default ;
    buffer_span() ;
    buffer_span( const container &bytes ) ;
    buffer_span( const_iterator first, const_iterator last ) ;
    buffer_span( const_iterator first, std::size_t count ) ;

    const_iterator begin() const ;
    const_iterator end() const ;
    const element_type *data() const ;
    const_reference front() const ;
    const_reference back() const ;
    const_reference operator[]( index_type index ) const ;
    const_reference at( index_type index ) const ;

    std::size_t size() const ;
    bool empty() const ;
    bool valid() const ;
    operator bool() const noexcept ;

    buffer_span subspan( index_type start ) const ;
    buffer_span subspan( index_type start, std::size_t count ) const ;

  private:
    const_iterator    _first{} ;
    const_iterator    _last{} ;
    bool              _isnull {false} ;
  };

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------

  /**
   *  @brief  buffer class
   */
  class buffer {
  public:
    using container = byte_array ;
    using element_type = byte_array::value_type ;
    using iterator = byte_array::iterator ;
    using const_iterator = byte_array::const_iterator ;
    using reverse_iterator = byte_array::reverse_iterator ;
    using const_reverse_iterator = byte_array::const_reverse_iterator ;
    using index_type = std::size_t ;
    using size_type = std::size_t ;
    using reference = byte_array::reference ;
    using const_reference = byte_array::const_reference ;
    using pointer = byte_array::pointer ;
    using const_pointer = byte_array::const_pointer ;

  public:
    /// No default constructor
    buffer() = delete ;
    /// No copy constructor
    buffer( const buffer& ) = delete ;
    /// No assignment by copy
    buffer& operator=( const buffer& ) = delete ;

    /**
     *  @brief  Constructor with buffer size
     *
     *  @param  len the buffer size to allocate
     */
    buffer( size_type len ) ;

    /**
     *  @brief  Constructor with byte array (move!)
     *
     *  @param  bytes the byte array
     */
    buffer( container &&bytes ) ;

    /**
     *  @brief  Move constructor
     *
     *  @param  rhs the other buffer object to move
     */
    buffer( buffer&& rhs ) ;

    /**
     *  @brief  Move assignment operator
     *
     *  @param  rhs the other buffer object to move
     */
    buffer& operator=( buffer&& rhs ) ;

    /**
     *  @name Iterators
     */
    ///@{
    /**
     *  @brief  Get an iterator to the beginning of the byte array
     */
    const_iterator begin() const ;

    /**
     *  @brief  Get an iterator to the end of the byte array
     */
    const_iterator end() const ;

    /**
     *  @brief  Get an iterator to the beginning of the byte array
     */
    iterator begin() ;

    /**
     *  @brief  Get an iterator to the end of the byte array
     */
    iterator end() ;

    /**
     *  @brief  Get an iterator to the reverse beginning (end) of the byte array
     */
    const_reverse_iterator rbegin() const ;

    /**
     *  @brief  Get an iterator to the reverse end (begin) of the byte array
     */
    const_reverse_iterator rend() const ;

    /**
     *  @brief  Get an iterator to the reverse beginning (end) of the byte array
     */
    reverse_iterator rbegin() ;

    /**
     *  @brief  Get an iterator to the reverse end (begin) of the byte array
     */
    reverse_iterator rend() ;
    ///@}

    /**
     *  @name Element access
     */
    ///@{
    /**
     *  @brief  Get a byte at the specified position
     *
     *  @param  index the index access
     */
    const_reference operator[]( index_type index ) const ;

    /**
     *  @brief  Get a byte at the specified position
     *
     *  @param  index the index access
     */
    reference operator[]( index_type index ) ;

    /**
     *  @brief  Get a byte at the specified position
     *
     *  @param  index the index access
     */
    const_reference at( index_type index ) const ;

    /**
     *  @brief  Get a byte at the specified position
     *
     *  @param  index the index access
     */
    reference at( index_type index ) ;

    /**
     *  @brief  Get a byte at the front position
     */
    const_reference front() const ;

    /**
     *  @brief  Get a byte at the front position
     */
    reference front() ;

    /**
     *  @brief  Get a byte at the back position
     */
    const_reference back() const ;

    /**
     *  @brief  Get a byte at the back position
     */
    reference back() ;

    /**
     *  @brief  Get the raw buffer bytes as an array
     */
    const_pointer data() const ;

    /**
     *  @brief  Get the raw buffer bytes as an array
     */
    pointer data() ;

    /**
     *  @brief  Get a pointer on the raw bytes at the given index
     *
     *  @param  index the index access
     */
    const_pointer ptr( index_type index ) const ;

    /**
     *  @brief  Get a pointer on the raw bytes at the given index
     *
     *  @param  index the index access
     */
    pointer ptr( index_type index ) ;
    ///@}

    /**
     *  @name Capacity
     */
    ///@{
    /**
     *  @brief  Get the buffer size
     */
    size_type size() const ;

    /**
     *  @brief  Whether the buffer is empty
     */
    bool empty() const ;
    ///@}

    /**
     *  @name Modifiers
     */
    ///@{
    /**
     *  @brief  Resize the buffer to the specified size
     *
     *  @param  newsize the new buffer size
     */
    void resize( size_type newsize ) ;

    /**
     *  @brief  Expand the buffer by adding new bytes
     *
     *  @param  nbytes the number of bytes to add
     *  @return the new buffer size
     */
    size_type expand( size_type nbytes ) ;

    /**
     *  @brief  Clear the buffer.
     *          Note that the memory is not released until the buffer
     *          gets deleted. To release the buffer memory, use shrink = true
     *
     * @param  shrink whether to shrink the buffer after the clear operation
     */
    void clear( bool shrink = false ) ;
    ///@}

    /**
     *  @name Operations
     */
    ///@{
    /**
     *  @brief  Write data in the buffer
     *
     *  @param  ptr the address of the bytes to write
     *  @param  position the target position in the buffer where to write
     *  @param  length the length of the byte element
     *  @param  count the number of elements to write
     *  @return the number of bytes written in the buffer
     */
    size_type write( const_pointer const ptr, index_type position, size_type length, size_type count ) ;

    /**
     *  @brief  Write data in the buffer
     *
     *  @param  ptr the address of the bytes to write
     *  @param  position the target position in the buffer where to write
     *  @param  count the number of elements to write
     *  @return the number of bytes written in the buffer
     */
    template <typename T>
    size_type write( const T *const ptr, index_type position, size_type count ) ;

    /**
     *  @brief  Read data from the buffer
     *
     *  @param  ptr the address to receive read bytes
     *  @param  position the target position in the buffer where to read
     *  @param  length the length of the byte element
     *  @param  count the number of elements to read
     *  @return the number of bytes read out from the buffer
     */
    size_type read( pointer ptr, index_type position, size_type length, size_type count ) const ;

    /**
     *  @brief  Read data from the buffer
     *
     *  @param  ptr the address to receive read bytes
     *  @param  position the target position in the buffer where to read
     *  @param  count the number of elements to read
     *  @return the number of bytes read out from the buffer
     */
    template <typename T>
    size_type read( T *ptr, index_type position, size_type count ) const ;

    /**
     *  @brief  Re-use the buffer bytes. Creates a new buffer object
     *          and move the byte container in the new buffer object.
     *          The current buffer is invalidated
     */
    buffer reuse() ;

    /**
     *  @brief  Whether the buffer is valid
     *
     *  The buffer may be invalidated after a move operation
     *  occuring when:
     *  - std::move is called
     *  - buffer::reuse() is called
     */
    bool valid() const ;

    /**
     *  @brief  Get a the entire buffer as buffer_span
     */
    buffer_span span() const ;

    /**
     *  @brief  Get a sub span of the buffer
     *
     *  @param  start where the span starts
     */
    buffer_span span( index_type start ) const ;

    /**
     *  @brief  Get a sub span of the buffer
     *
     *  @param  start where the sub span starts
     *  @param  count the length of the sub span
     */
    buffer_span span( index_type start, size_type count ) const ;
    ///@}

  private:
    ///< The byte array container
    container           _bytes {} ;
    ///< Whether the buffer is in a valid state
    bool                _valid {true} ;
  };

}

#include <sio/details/buffer_impl.h>
