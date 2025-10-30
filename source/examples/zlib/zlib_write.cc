// -- sio headers
#include <sio/definitions.h>
#include <sio/exception.h>
#include <sio/api.h>
#include <sio/compression/zlib.h>
#include <sio/buffer.h>
// -- sio examples headers
#include <sioexamples/data.h>
#include <sioexamples/blocks.h>
#include <iostream>
#include <memory>
#include <string>


/**
 *  This example illustrate how to write a simple data structure and 
 *  compress it using zlib.
 *  The sio block writing function is available in sioexamples/blocks.h 
 */
int main( int argc, char **argv ) {
  
  // place the whole code in a try-catch block.
  // sio provides an exception class (sio::exception)
  try {
    // the .sio extension is not important here.
    // it just helps in identiying the file name clearly in these examples
    const std::string fname = (argc > 1) ? argv[1] : "particle_compressed.sio" ;
    
    // Open a stream for writing.
    // sio::ofstream is a simple typedef of a standard std file stream.
    // Nevertheless, we are dealing with binary data, you need to open 
    // it with the appropriate mode  
    sio::ofstream stream ;
    stream.open( fname , std::ios::binary ) ;
    if( not stream.is_open() ) {
      // These macros catch the line, current file name and function
      // in which the exception is thrown, and throw an exception
      // with code and message
      SIO_THROW( sio::error_code::not_open, "Couldn't open output stream '" + fname + "'" ) ;
    }
    
    /// SIO write data in records, formed of a list of blocks.
    /// This example illustrates how to create a simple block
    /// containing a simple data structure, a particle.
    /// We first create our particle block and add it to the list
    sio::block_list blocks {} ;
    auto part_blk = std::make_shared<sio::example::particle_block>() ;
    blocks.push_back( part_blk ) ;
    
    /// Fill some data in our particle
    sio::example::particle part ;
    part._energy = 42.f ;
    part._pid = 12 ;
    part._x = 0.01 ;
    part._y = 0.02 ;
    part._z = 0.03 ;
    part_blk->set_particle( part ) ;
    
    /// The record is first written in a buffer before being written 
    /// in a file. Let's create a buffer that will hold our record data.
    /// The buffer constructor requires a initial buffer size. Let's 
    /// give something reasonable there ...
    sio::buffer buf( sio::kbyte ) ;
    
    /// We can write our record there. The first argument is the record name.
    /// You can write multiple times a record with the same name if you wish.
    /// The returned strcture contains information on the record you've just 
    /// written in the buffer 
    auto rec_info = sio::api::write_record( "particle_record", buf, blocks, 0 ) ;

    /// There we use zlib to compress the record into another buffer
    sio::buffer compbuf( sio::kbyte ) ;
    sio::zlib_compression compressor ;
    compressor.set_level( 1 ) ;
    sio::api::compress_record( rec_info, buf, compbuf, compressor ) ;
    
    /// The compressed record is ready to be written in the file !
    /// Here we pass two buffer arguments, one is the record header which
    /// is not compressed and the second is the full compressed record data
    sio::api::write_record( stream, buf.span(0, rec_info._header_length), compbuf.span(), rec_info ) ;
    
    /// A bit of cleanup and we are done !
    stream.close() ;
    
    std::cout << "Written sio file " << fname << std::endl ;
  }
  catch( sio::exception &e ) {
    std::cout << "Caught sio exception :\n" << e.what() << std::endl ;
  }
  
  return 0 ;
}
