// -- sio headers
#include <sio/definitions.h>
#include <sio/buffer.h>
#include <sio/api.h>
#include <sio/compression/zlib.h>

// -- std headers
#include <limits>

/**
 *  @brief  Utility in sio to dump all records from a file on disk
 */
int main( int argc, char **argv ) {
  
  if( (argc < 2) or (argc > 4) ) {
    std::cout << "Dump detailed SIO record info from file to console" << std::endl ;
    std::cout << "Usage: " << std::endl ;
    std::cout << "- Dump all records from the file:" << std::endl ;
    std::cout << "     sio-dump-detailed <file-name>" << std::endl ;
    std::cout << "- Dump the n first records from the file:" << std::endl ;
    std::cout << "     sio-dump-detailed <file-name> <n>" << std::endl ;
    std::cout << "- Skip the m first record and dump the n next records from the file:" << std::endl ;
    std::cout << "     sio-dump-detailed <file-name> <m> <n>" << std::endl ;
    return 1 ;
  }
  
  const std::string fname = argv[1] ;
  unsigned int count = std::numeric_limits<unsigned int>::max() ;
  unsigned int skip = 0 ;
  
  if( argc > 3 ) {
    skip = atoi(argv[2]) ;
    count = atoi(argv[3]) ;
    std::cout << "Will skip " << skip << " records and dump the " << count << " next ones ..." << std::endl ;
  }
  else if( argc > 2 ) {
    count = atoi(argv[2]) ;
    std::cout << "Will dump only " << count << " records ..." << std::endl ;
  }
  
  sio::ifstream file;
  file.open( fname , std::ios::in | std::ios::binary ) ;
  sio::api::dump_records( file, skip, count, true ) ;
  
  return 0 ;
}

