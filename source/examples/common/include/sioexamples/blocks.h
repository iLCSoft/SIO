#pragma once

#include <sio/block.h>
#include <sio/version.h>
#include <sio/io_device.h>
#include <sioexamples/data.h>

namespace sio {
  
  namespace example {
    
    /**
     *  @brief  particle_block class.
     *  
     *  Illustrates the reading and writing of a simple data structure 
     *  in a block with sio. 
     *  
     *  The constructor must provide the name and 
     *  the version of the block. To encode a version, use the helper 
     *  function sio::version::encode_version(maj, min).
     *
     *  The read and write functions simply write all members of the 
     *  particle structure. Note that the structure can't be directly
     *  written in a single shot, as sio deals with endianess and padding
     *  on io operations. Thus the data must serialized one by one.
     *
     *  Note that the read and write functions here use the macro 
     *  SIO_DATA which understand whether you are reading or writing data
     *  depending if you use a read_device or a write device. You can then
     *  notice that the read and write function, in this simple case are 
     *  identical. See particle_data() function defined in sioexamples/data.h
     */
    class particle_block : public sio::block {
    public:
      particle_block() :
        sio::block( "particle", sio::version::encode_version( 1, 2 ) ) {
        /* nop */
      }
    
      const particle &get_particle() const { return _particle ; }
      void set_particle( const particle &part ) { _particle = part ; }
    
      // Read the particle data from the device
      void read( sio::read_device &device, sio::version_type /*vers*/ ) override {
        particle_data( _particle, device ) ;
      }
    
      // Write the particle data to the device
      void write( sio::write_device &device ) override {
        particle_data( _particle, device ) ;
      }
    
    private:
      ///< The particle data to read/write
      particle            _particle {} ;
    };
    
    
    class linked_list_block : public sio::block {
    public:
      linked_list_block() :
        sio::block( "linked_list", sio::version::encode_version( 1, 2 ) ) {
        /* nop */
      }
    
      std::shared_ptr<linked_list> root() const { return _root ; }
      void set_root( std::shared_ptr<linked_list> r ) { _root = r ; }
    
      // Read the linked_list data from the device
      void read( sio::read_device &device, sio::version_type /*vers*/ ) override {
        int nlinks = 0 ;
        // read the number of elements in the linked list
        SIO_SDATA( device, nlinks ) ;
        // std::cout << "Read " << nlinks << " links" << std::endl ;
        // create the root element
        _root = std::make_shared<linked_list>() ;
        auto current = _root ;
        for( int i=0 ; i<nlinks ; i++ ) {
          linked_list_data( current, device ) ;
          // last element ? then don't allocate the next of the list
          if( i+1 < nlinks ) {
            current->_next = std::make_shared<linked_list>() ;
          }
          current = current->_next ;     
        }
      }
    
      // Write the linked_list data to the device
      void write( sio::write_device &device ) override {
        int nlinks = 0 ;
        auto current = _root ;
        while( nullptr != current ) {
          nlinks++ ;
          current = current->_next ;
        }
        // write the number of elements in the linked list
        SIO_SDATA( device, nlinks ) ;
        current = _root ;
        while( nullptr != current ) {
          linked_list_data( current, device ) ;
          current = current->_next ;
        }
      }
    
    private:
      ///< The linked_list data to read/write
      std::shared_ptr<linked_list>            _root {nullptr} ;
    };
    
  }
  
}
