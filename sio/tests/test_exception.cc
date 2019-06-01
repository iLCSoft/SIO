#include <SIO_testing.h>
#include <sio/definitions.h>
#include <sio/exception.h>
#include <sio/buffer.h>
#include <sio/compression.h>
#include <sio/compression/zlib.h>

using UnitTest = sio::test::UnitTest;

int main() {

  UnitTest unitTest("test_exception");

  try {
    try {
      SIO_THROW( sio::error_code::invalid_argument, "Test of invalid argument" );
    }
    catch( sio::exception &e ) {
      SIO_RETHROW( e, sio::error_code::io_failure, "Test of io failure" );
    }
  }
  catch( sio::exception &e ) {
    std::cout << "Caught sio exception: \n" << e.what() << std::endl ;
  }
  
  return 0;
}