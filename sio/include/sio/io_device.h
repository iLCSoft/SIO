#pragma once

// -- sio headers
#include <sio/definitions.h>
#include <sio/buffer.h>

namespace sio {

  /**
   *  @brief  read_device class.
   *
   *  Holds a buffer_span and a cursor allowing to read 
   *  data sequentially, moving the cursor after reading.
   */
  class read_device {
  public:
    using cursor_type = std::size_t ;
    using size_type = std::size_t ;

  public:
    /// Default constructor
    read_device() = default ;
    /// Default copy constructor
    read_device( const read_device & ) = default ;
    /// Default move constructor
    read_device( read_device && ) = default ;
    /// Default assignement operator
    read_device& operator=( const read_device & ) = default ;
    /// Default move assignement operator
    read_device& operator=( read_device && ) = default ;
    /// Default destructor
    ~read_device() = default ;
    
    /**
     *  @brief  Constructor with buffer span
     * 
     *  @param  buf the input buffer span
     */
    read_device( buffer_span buf ) ;
    
    /**
     *  @name Buffer
     */
    ///{@
    /**
     *  @brief  Set the buffer span to use (move)
     * 
     *  @param  buf the buffer to use
     */
    void set_buffer( buffer_span &&buf ) ;
    
    /**
     *  @brief  Set the buffer span to use (copy)
     * 
     *  @param  buf the buffer to use
     */
    void set_buffer( const buffer_span &buf ) ;
    ///@}

    /**
     *  @name Cursor
     */
    ///{@
    /**
     *  @brief  Get the cursor position
     */
    cursor_type position() const ;
    
    /**
     *  @brief  Seek the cursor at a given position
     * 
     *  @param  pos the new cursor position
     */
    void seek( cursor_type pos ) ;
    ///@}

    /**
     *  @name I/O operations
     */
    ///{@
    /**
     *  @brief  Read out a variable from the buffer. Move the cursor accordingly
     * 
     *  @param  var the variable to receive
     */
    template <typename T>
    void read( T &var ) ;

    /**
     *  @brief  Read out an array of variables from the buffer. Move the cursor accordingly
     * 
     *  @param  var the address of the array
     *  @param  count the number of element to read out
     */
    template <typename T>
    void read( T *var, size_type count ) ;
    
    /**
     *  @brief  Read out a "pointer to" pointer from the buffer.
     *          A new entry is created for a future relocation.
     *          The pointer relocation is performed at the end of record reading
     *          
     *  @param  ptr the address to register
     */
    void read_pointer_to( ptr_type *ptr ) ;
    
    /**
     *  @brief  Read out a "pointed at" pointer from the buffer.
     *          A new entry is created for a future relocation.
     *          The pointer relocation is performed at the end of record reading
     *          
     *  @param  ptr the address to register
     */
    void read_pointed_at( ptr_type *ptr ) ;
    
    /**
     *  @brief  Perform the pointer relocation after the whole record has
     *          been read. The pointers are relocated and the pointer maps
     *          are cleared
     */
    void pointer_relocation() ;
    ///@}

  private:
    ///< The buffer span
    buffer_span         _buffer {} ;
    ///< The device cursor
    cursor_type         _cursor {0} ;
    ///< The map of pointer "pointed at"
    pointed_at_map      _pointed_at {} ;
    ///< The map of pointer "pointer to"
    pointer_to_map      _pointer_to {} ;
  };

}

#include <sio/details/io_device_impl.h>
