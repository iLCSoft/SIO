// -- sio headers
#include <sio/definitions.h>
#include <sio/exception.h>
#include <sio/api.h>

// -- sio examples headers
#include <sioexamples/data.h>
#include <sioexamples/blocks.h>

/**
 *  This example illustrate how to read a simple data structure.
 *  The sio block reading function is available in sioexamples/blocks.h 
 */
int main( int argc, char **argv ) {
  
  // place the whole code in a try-catch block.
  // sio provides an exception class (sio::exception)
  try {
    // the .sio extension is not important here.
    // it just helps in identiying the file name clearly in these examples
    const std::string fname = (argc > 1) ? argv[1] : "particle.sio" ;
    
    // Open a stream for writing.
    // sio::ifstream is a simple typedef of a standard std file stream.
    // Nevertheless, we are dealing with binary data, you need to open 
    // it with the appropriate mode  
    sio::ifstream stream ;
    stream.open( fname , std::ios::binary ) ;
    if( not stream.is_open() ) {
      // These macros catch the line, current file name and function
      // in which the exception is thrown, and throw an exception
      // with code and message
      SIO_THROW( sio::error_code::not_open, "Couldn't open input stream '" + fname + "'" ) ;
    }
    
    /// We first extract the buffer out of the stream
    /// We do this in two steps for the example
    /// First the record header (info)
    /// Second the record data
    sio::record_info rec_info ;
    sio::buffer info_buffer( sio::max_record_info_len ) ;
    sio::buffer rec_buffer( sio::kbyte ) ;
    sio::api::read_record_info( stream, rec_info, info_buffer ) ;
    sio::api::read_record_data( stream, rec_info, rec_buffer ) ;
    
    /// SIO write data in records, formed of a list of blocks.
    /// This example illustrates how to create a simple block
    /// containing a simple data structure, a particle.
    /// We first create our particle block and add it to the list
    sio::block_list blocks {} ;
    auto part_blk = std::make_shared<sio::example::particle_block>() ;
    blocks.push_back( part_blk ) ;
    
    /// We can now decode the record buffer using the block list.
    /// The method span returns a read-only object on the buffer
    /// on a specific range. You never know what is the output 
    /// buffer size after SIO calls, so it is recommended to 
    /// specify the range of the blocks decoding. Here from the
    /// beginning to the record data length.
    sio::api::read_blocks( rec_buffer.span( 0, rec_info._data_length ), blocks ) ;

    /// Let see what we got from the record:
    auto part = part_blk->get_particle() ;
    std::cout << "Read particle:" << std::endl ;
    std::cout << "pid:    " << part._pid << std::endl ;
    std::cout << "energy: " << part._energy << std::endl ;
    std::cout << "x:      " << part._x << std::endl ;
    std::cout << "y:      " << part._y << std::endl ;
    std::cout << "z:      " << part._z << std::endl ;
    
    /// A bit of cleanup and we are done !
    stream.close() ;
    
    std::cout << "Read sio file " << fname << std::endl ;
  }
  catch( sio::exception &e ) {
    std::cout << "Caught sio exception :\n" << e.what() << std::endl ;
  }
  
  return 0 ;
}