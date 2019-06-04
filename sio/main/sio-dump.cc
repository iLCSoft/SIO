// -- sio headers
#include <sio/definitions.h>
#include <sio/helpers.h>

// -- std headers
#include <iostream>
#include <limits>
#include <string>

/**
 *  @brief  Utility in sio to dump all records from a file on disk
 */
int main( int argc, char **argv ) {
  
  if( (argc < 2) or (argc > 4) ) {
    std::cout << "Usage: " << std::endl ;
    std::cout << "- Dump all records from the file:" << std::endl ;
    std::cout << "     sio-dump <file-name>" << std::endl ;
    std::cout << "- Dump the n first records from the file:" << std::endl ;
    std::cout << "     sio-dump <file-name> <n>" << std::endl ;
    std::cout << "- Skip the m first record and dump the n next records from the file:" << std::endl ;
    std::cout << "     sio-dump <file-name> <m> <n>" << std::endl ;
    return 1 ;
  }
  
  const std::string fname = argv[1] ;
  unsigned int max_record_number = std::numeric_limits<unsigned int>::max() ;
  unsigned int skip_record_number = 0 ;
  if( argc > 3 ) {
    skip_record_number = atoi(argv[2]) ;
    max_record_number = skip_record_number + atoi(argv[3]) ;
    std::cout << "Will skip " << skip_record_number << " records and dump the " << atoi(argv[3]) << " next ones ..." << std::endl ;
  }
  else if( argc > 2 ) {
    max_record_number = atoi(argv[2]) ;
    std::cout << "Will dump only " << max_record_number << " records ..." << std::endl ;
  }
  unsigned int record_counter (0) ;
  
  try {
    sio::ifstream file;
    file.open( fname , std::ios::in | std::ios::binary ) ;
    try {
      sio::record_info rec_info ;
      sio::buffer rec_buffer( sio::max_record_info_len ) ;
      while(1) {
        if( record_counter >= max_record_number ) {
          break ;
        }
        sio::io_helper::read_record_info( file, rec_info, rec_buffer ) ;
        // seek after the record to read the next record info
        file.seekg( rec_info._file_end ) ;
        ++ record_counter ;
        // skip printing if requested so
        if( (skip_record_number > 0) and skip_record_number >= record_counter ) {
          continue ;
        }
        std::cout << "======== SIO record ========" << std::endl ;
        std::cout << rec_info << std::endl ;
      }
    }
    catch( sio::exception &e ) {
      // we are finished !
      if( e.code() == sio::error_code::eof ) {
        return 0 ;
      }
      throw e ;
    }
  }
  catch( sio::exception &e ) {
    std::cout << "Caught SIO exception:" << std::endl ;
    std::cout << e.what() << std::endl ;
  }
  
  return 0 ;
}

