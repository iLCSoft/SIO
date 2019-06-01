#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {
  
  class buffer ;
  
  /**
   *  @brief  buffer_view class
   *
   *  Wrapper of a buffer providing only const methods to it
   */
  class buffer_view {
  public:
    /// No default constructor
    buffer_view() = delete ;
    
    /**
     *  @brief  Constructor with buffer object
     * 
     *  @param  buf the buffer to wrap
     */
    buffer_view( const buffer &buf ) ;
    
    /**
     *  @name Iterators
     */
    ///@{
    /**
     *  @brief  Get an iterator to the beginning of the byte array
     */
    byte_array::const_iterator begin() const ;
    
    /**
     *  @brief  Get an iterator to the end of the byte array
     */
    byte_array::const_iterator end() const ;
    
    /**
     *  @brief  Get an iterator to the reverse beginning (end) of the byte array
     */
    byte_array::const_reverse_iterator rbegin() const ;
    
    /**
     *  @brief  Get an iterator to the reverse end (begin) of the byte array
     */
    byte_array::const_reverse_iterator rend() const ;
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
    const byte &operator[]( index_type index ) const ;
    
    /**
     *  @brief  Get a byte at the front position
     */
    const byte &front() const ;
    
    /**
     *  @brief  Get a byte at the back position
     */
    const byte &back() const ;
    
    /**
     *  @brief  Get a byte at the specified position
     * 
     *  @param  index the index access
     */
    const byte &at( index_type index ) const ;
    
    /**
     *  @brief  Get the raw buffer bytes as an array
     */
    const byte *data() const ;
    
    /**
     *  @brief  Get a pointer on the raw bytes at the given index
     *  
     *  @param  index the index access
     */
    const byte *ptr( index_type index ) const ;
    ///@}
    
    /**
     *  @name Capacity
     */
    ///@{
    /**
     *  @brief  Get the buffer size
     */
    std::size_t size() const ;
    
    /**
     *  @brief  Whether the buffer is empty
     */
    bool empty() const ;
    
    /**
     *  @brief  Whether the buffer is valid
     *
     *  The buffer may be invalidated after a move operation 
     *  occuring when:
     *  - std::move is called
     *  - buffer::reuse() is called
     */
    bool valid() const ;
    ///@}
    
    /**
     *  @name Operations
     */
    ///@{
    /**
     *  @brief  Read data from the buffer
     *  
     *  @param  ptr the address to receive read bytes
     *  @param  position the target position in the buffer where to read
     *  @param  length the length of the byte element
     *  @param  count the number of elements to read
     *  @return the number of bytes read out from the buffer
     */
    std::size_t read( byte *ptr, index_type position, std::size_t length, std::size_t count ) const ;
    
    /**
     *  @brief  Read data from the buffer
     *  
     *  @param  ptr the address to receive read bytes
     *  @param  position the target position in the buffer where to read
     *  @param  count the number of elements to read
     *  @return the number of bytes read out from the buffer
     */
    template <typename T>
    std::size_t read( T *ptr, index_type position, std::size_t count ) const ;
    ///@}
    
  private:
    ///< The buffer on which the view is acting
    const buffer       &_buffer ;
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  /**
   *  @brief  buffer class
   */
  class buffer {
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
    buffer( std::size_t len ) ;
    
    /**
     *  @brief  Constructor with byte array (move!)
     *  
     *  @param  bytes the byte array
     */
    buffer( byte_array &&bytes ) ;
    
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
    byte_array::const_iterator begin() const ;
    
    /**
     *  @brief  Get an iterator to the end of the byte array
     */
    byte_array::const_iterator end() const ;
    
    /**
     *  @brief  Get an iterator to the beginning of the byte array
     */
    byte_array::iterator begin() ;
    
    /**
     *  @brief  Get an iterator to the end of the byte array
     */
    byte_array::iterator end() ;
    
    /**
     *  @brief  Get an iterator to the reverse beginning (end) of the byte array
     */
    byte_array::const_reverse_iterator rbegin() const ;
    
    /**
     *  @brief  Get an iterator to the reverse end (begin) of the byte array
     */
    byte_array::const_reverse_iterator rend() const ;
    
    /**
     *  @brief  Get an iterator to the reverse beginning (end) of the byte array
     */
    byte_array::reverse_iterator rbegin() ;
    
    /**
     *  @brief  Get an iterator to the reverse end (begin) of the byte array
     */
    byte_array::reverse_iterator rend() ;
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
    const byte &operator[]( index_type index ) const ;
    
    /**
     *  @brief  Get a byte at the specified position
     * 
     *  @param  index the index access
     */
    byte &operator[]( index_type index ) ;
    
    /**
     *  @brief  Get a byte at the specified position
     * 
     *  @param  index the index access
     */
    const byte &at( index_type index ) const ;
    
    /**
     *  @brief  Get a byte at the specified position
     * 
     *  @param  index the index access
     */
    byte &at( index_type index ) ;
    
    /**
     *  @brief  Get a byte at the front position
     */
    const byte &front() const ;
    
    /**
     *  @brief  Get a byte at the front position
     */
    byte &front() ;
    
    /**
     *  @brief  Get a byte at the back position
     */
    const byte &back() const ;
    
    /**
     *  @brief  Get a byte at the back position
     */
    byte &back() ;
    
    /**
     *  @brief  Get the raw buffer bytes as an array
     */
    const byte *data() const ;
    
    /**
     *  @brief  Get the raw buffer bytes as an array
     */
    byte *data() ;
    
    /**
     *  @brief  Get a pointer on the raw bytes at the given index
     *  
     *  @param  index the index access
     */
    const byte *ptr( index_type index ) const ;
    
    /**
     *  @brief  Get a pointer on the raw bytes at the given index
     *  
     *  @param  index the index access
     */
    byte *ptr( index_type index ) ;
    ///@}
    
    /**
     *  @name Capacity
     */
    ///@{
    /**
     *  @brief  Get the buffer size
     */
    std::size_t size() const ;
    
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
    void resize( std::size_t newsize ) ;
    
    /**
     *  @brief  Expand the buffer by adding new bytes
     * 
     *  @param  nbytes the number of bytes to add
     *  @return the new buffer size
     */
    std::size_t expand( std::size_t nbytes ) ;
    
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
    std::size_t write( const byte *const ptr, index_type position, std::size_t length, std::size_t count ) ;
    
    /**
     *  @brief  Write data in the buffer
     *  
     *  @param  ptr the address of the bytes to write
     *  @param  position the target position in the buffer where to write
     *  @param  count the number of elements to write
     *  @return the number of bytes written in the buffer
     */
    template <typename T>
    std::size_t write( const T *const ptr, index_type position, std::size_t count ) ;
    
    /**
     *  @brief  Read data from the buffer
     *  
     *  @param  ptr the address to receive read bytes
     *  @param  position the target position in the buffer where to read
     *  @param  length the length of the byte element
     *  @param  count the number of elements to read
     *  @return the number of bytes read out from the buffer
     */
    std::size_t read( byte *ptr, index_type position, std::size_t length, std::size_t count ) const ;
    
    /**
     *  @brief  Read data from the buffer
     *  
     *  @param  ptr the address to receive read bytes
     *  @param  position the target position in the buffer where to read
     *  @param  count the number of elements to read
     *  @return the number of bytes read out from the buffer
     */
    template <typename T>
    std::size_t read( T *ptr, index_type position, std::size_t count ) const ;
    
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
    ///@}

  private:
    ///< The byte array container
    byte_array          _bytes {} ;
    ///< Whether the buffer is in a valid state
    bool                _valid {true} ;
  };

}

#include <sio/details/buffer_impl.h>
