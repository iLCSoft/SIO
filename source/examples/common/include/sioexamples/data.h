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
    // data structure with a pointer. The shared_ptr
    // is just there to make the ownership clearer
    struct linked_list {
      std::string                      _name {""} ;
      std::shared_ptr<linked_list>     _next {nullptr} ;
    };
    
    /// Read or write linked_list data with the device
    template <typename devT>
    inline void linked_list_data( std::shared_ptr<linked_list> l, devT &device ) {
      // read/write name field
      // std::cout << "Totoooooooo" << std::endl ;
      SIO_SDATA( device, l->_name ) ;
      // std::cout << "ll data name: " << l->_name << std::endl ;
      // read/write a pointer member. This object won't allocated on read
      // but relocated after all data have been read from a record
      auto ptr = l->_next.get() ;
      // std::cout << "ll reading pntr" << std::endl ;
      SIO_PNTR( device, &ptr ) ;
      // the linked list pointer itself can be referenced by other structures
      // read/write the address of this object
      // std::cout << "ll reading ptag" << std::endl ;
      SIO_PTAG( device, l.get() ) ;

    }
    
  }
  
}





