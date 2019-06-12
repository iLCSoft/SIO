#pragma once

// -- std headers
#include <string>

namespace sio {
  
  namespace example {

    // simple data structure holding particle information
    struct particle {
      int           _pid {0} ;
      float         _energy {0.f} ;
      float         _x {0.f} ;
      float         _y {0.f} ;
      float         _z {0.f} ;
    };
    
    /// Read or write particle data with the device
    template <typename devT>
    inline void particle_data( particle &part, devT &device ) {
      SIO_DATA( device, &part._pid, 1 ) ;
      SIO_DEBUG( "Reading/writing particle, pid: " << part._pid ) ;
      SIO_DATA( device, &part._energy, 1 ) ;
      SIO_DEBUG( "Reading/writing particle, energy: " << part._energy ) ;
      SIO_DATA( device, &part._x, 1 ) ;
      SIO_DEBUG( "Reading/writing particle, x: " << part._x ) ;
      SIO_DATA( device, &part._y, 1 ) ;
      SIO_DEBUG( "Reading/writing particle, y: " << part._y ) ;
      SIO_DATA( device, &part._z, 1 ) ;
      SIO_DEBUG( "Reading/writing particle, z: " << part._z ) ;
    }
    
    // simple example of data referencing another 
    // data structure with a pointer. Only the first 
    // element of the list is owned by the user
    struct linked_list {
      // destructor deletes the next element
      ~linked_list() {
        if( nullptr != _next ) {
          delete _next ;
        }
      }
      std::string       _name {} ;
      linked_list      *_next {nullptr} ;
    };
    
  }
  
}





