// -- sio headers
#include <sio/definitions.h>
#include <sio/exception.h>
#include <sio/api.h>

// -- sio examples headers
#include <sioexamples/data.h>
#include <sioexamples/blocks.h>

/**
 *  This example illustrate how to write a simple data structure.
 *  The sio block writing function is available in sioexamples/blocks.h 
 */
int main( int argc, char **argv ) {
  
  // place the whole code in a try-catch block.
  // sio provides an exception class (sio::exception)
  try {
    // the .sio extension is not important here.
    // it just helps in identiying the file name clearly in these examples
    const std::string fname = (argc > 1) ? argv[1] : "relocation.sio" ;
    
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
    auto ll_blk = std::make_shared<sio::example::linked_list_block>() ;
    blocks.push_back( ll_blk ) ;
    
    /// Create a linked list of 200 elements
    int n = 200 ;
    auto llroot = std::make_shared<sio::example::linked_list>() ;
    auto llcur = llroot ;
    for( int i=0 ; i<n ; i++ ) {
      llcur->_name = "element_" + std::to_string( i ) ;
      std::cout << "Created element in linked list with name: " << llcur->_name << std::endl ;
      if( i+1 < n ) {
        std::cout << "Creating next element..." << std::endl ;
        llcur->_next = std::make_shared<sio::example::linked_list>() ;
      }
      else {
        std::cout << "It was the last element" << std::endl ;
      }
      llcur = llcur->_next ;
    }

    ll_blk->set_root( llroot ) ;
    
    /// The record is first written in a buffer before being written 
    /// in a file. Let's create a buffer that will hold our record data.
    /// The buffer constructor requires a initial buffer size. Let's 
    /// give something reasonable there ...
    sio::buffer buf( sio::kbyte ) ;
    
    /// We can write our record there. The first argument is the record name.
    /// You can write multiple times a record with the same name if you wish.
    /// The returned strcture contains information on the record you've just 
    /// written in the buffer 
    auto rec_info = sio::api::write_record( "linked_list_record", buf, blocks, 0 ) ;
    
    /// The record is ready to be written in the file !
    sio::api::write_record( stream, buf.span(), rec_info ) ;
    
    /// A bit of cleanup and we are done !
    stream.close() ;
    
    std::cout << "Written sio file " << fname << std::endl ;
  }
  catch( sio::exception &e ) {
    std::cout << "Caught sio exception :\n" << e.what() << std::endl ;
  }
  
  return 0 ;
}