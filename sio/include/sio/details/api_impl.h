#include <sio/exception.h>
#include <sio/buffer.h>
#include <sio/io_device.h>

namespace sio {

  template <class bufT, typename T>
  inline typename bufT::size_type api::read( const bufT &buffer, T *ptr, typename bufT::index_type position, typename bufT::size_type count ) {
    return api::read( buffer, SIO_BYTE_CAST(ptr), sizeof(T), position, count ) ;
  }

  //--------------------------------------------------------------------------

  template <class bufT>
  inline typename bufT::size_type api::read( const bufT &buffer, typename bufT::pointer ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) {
    if( not buffer.valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    const auto bytelen = length*count ;
    const auto padlen = (bytelen + sio::padding) & sio::padding_mask ;
    SIO_DEBUG( "Reading... len: " << length << ", count: " << count << ", padlen: " << padlen ) ;
    if( position + padlen > buffer.size() ) {
      std::stringstream ss ;
      ss << "Can't read " << padlen << " bytes out of buffer (pos=" << position << ")" ;
      SIO_THROW( sio::error_code::invalid_argument, ss.str() ) ;
    }
    auto ptr_read = buffer.ptr( position ) ;
    sio::memcpy_helper::copy( SIO_CUCHAR_CAST(ptr_read), SIO_UCHAR_CAST(ptr), length, count ) ;
    return padlen ;
  }

  //--------------------------------------------------------------------------

  template <class bufT, typename T>
  inline typename bufT::size_type api::write( bufT &buffer, const T *const ptr, typename bufT::index_type position, typename bufT::size_type count ) {
    return api::write( buffer, SIO_CBYTE_CAST(ptr), sizeof(T), position, count ) ;
  }

  //--------------------------------------------------------------------------

  template <class bufT>
  inline typename bufT::size_type api::write( bufT &buffer, typename bufT::const_pointer const ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) {
    if( not buffer.valid() ) {
      SIO_THROW( sio::error_code::bad_state, "Buffer is invalid." ) ;
    }
    const auto bytelen = length*count ;
    const auto padlen = (bytelen + sio::padding) & sio::padding_mask ;
    if( position + padlen >= buffer.size() ) {
      buffer.expand( padlen ) ;
    }
    auto ptr_write = buffer.ptr( position ) ;
    sio::memcpy_helper::copy( SIO_CUCHAR_CAST(ptr), SIO_UCHAR_CAST(ptr_write), length, count ) ;
    for( auto bytcnt = bytelen; bytcnt < padlen; bytcnt++ ) {
      *(ptr_write + bytcnt) = sio::null_byte ;
    }
    return padlen ;
  }

  //--------------------------------------------------------------------------

  inline void api::read_record_info( sio::ifstream &stream, record_info &rec_info, buffer &outbuf ) {
    if( not stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "ifstream is not open!" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state!" ) ;
    }
    rec_info._file_start = stream.tellg() ;
    if( outbuf.size() < sio::max_record_info_len ) {
      auto mis_len = outbuf.size() - sio::max_record_info_len ;
      outbuf.resize( mis_len ) ;
    }
    stream.read( outbuf.data(), 8 ) ;
    if( stream.eof() ) {
      SIO_THROW( sio::error_code::eof, "Reached end of file !" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after reading first record bytes!" ) ;
    }
    unsigned int marker(0) ;
    read_device device( outbuf.span() ) ;
    // Interpret: 1) The length of the record header.
    //            2) The record marker.
    device.read( rec_info._header_length ) ;
    device.read( marker ) ;
    if( marker != sio::record_marker ) {
      stream.setstate( sio::ifstream::failbit ) ;
      SIO_THROW( sio::error_code::no_marker, "Record marker not found!" ) ;
    }
    // Interpret: 3) The options word.
    //            4) The length of the record data (compressed).
    //            5) The length of the record name (uncompressed).
    //            6) The length of the record name.
    //            7) The record name.
    stream.read( outbuf.ptr(8), rec_info._header_length-8 ) ;
    device.seek( 8 ) ;
    device.read( rec_info._options ) ;
    device.read( rec_info._data_length ) ;
    device.read( rec_info._uncompressed_length ) ;
    unsigned int name_length(0) ;
    device.read( name_length ) ;
    if( name_length > sio::max_record_name_len ) {
      SIO_THROW( sio::error_code::no_marker, "Invalid record name size (limited)" ) ;
    }
    rec_info._name.assign( name_length, '\0' ) ;
    device.read( &(rec_info._name[0]), name_length ) ;
    // a bit of debugging ...
    SIO_DEBUG( "=== Read record info ====" ) ;
    SIO_DEBUG( info ) ;
    const auto compressed = sio::compression_helper::is_compressed( rec_info._options ) ;
    // if the record is compressed skip the read pointer over 
    // any padding bytes that may have been inserted to make 
    // the next record header start on a four byte boundary in the file.
    auto tot_len = rec_info._data_length + rec_info._header_length ;
    if( compressed ) {
      tot_len += ((4 - (rec_info._data_length & sio::bit_align)) & sio::bit_align) ;
    }
    rec_info._file_end = rec_info._file_start ;
    rec_info._file_end += tot_len ;
  }

  //--------------------------------------------------------------------------

  inline void api::read_record_data( sio::ifstream &stream, const record_info &rec_info, buffer &outbuf, std::size_t buffer_shift ) {
    if( not stream.is_open() ) {
      SIO_THROW( sio::error_code::not_open, "ifstream is not open!" ) ;
    }
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state!" ) ;
    }
    // if the user provide large enough buffer, there is maybe no need to expand it
    auto total_len = rec_info._data_length + rec_info._header_length ;
    if( outbuf.size() + buffer_shift < total_len ) {
      auto mis_len = outbuf.size() - (total_len + buffer_shift) ;
      outbuf.expand( mis_len ) ;
    }
    // go to record start
    auto seek_pos = rec_info._file_start ;
    seek_pos += rec_info._header_length ;
    stream.seekg( seek_pos ) ;
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after a seek operation!" ) ;
    }
    stream.read( outbuf.ptr( buffer_shift ), rec_info._data_length ) ;
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::io_failure, "ifstream is in a bad state after a read operation!" ) ;
    }
    const auto compressed = sio::compression_helper::is_compressed( rec_info._options ) ;
    // if the record is compressed skip the read pointer over 
    // any padding bytes that may have been inserted to make 
    // the next record header start on a four byte boundary in the file.
    if( compressed ) {
      auto pad_length = (4 - (rec_info._data_length & sio::bit_align)) & sio::bit_align ;
      if( pad_length ) {
        if( not stream.seekg( pad_length, std::ios_base::cur ).good() ) {
          SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after a seek operation!" ) ;
        }
      }
    }
  }

  //--------------------------------------------------------------------------

  inline void api::read_record( sio::ifstream &stream, record_info &rec_info, buffer &outbuf ) {
    // read out the record info
    api::read_record_info( stream, rec_info, outbuf ) ;
    // read out the record data in the buffer. Shift the buffer position by the
    // size of the record header to keep the record header bytes in the buffer too.
    api::read_record_data( stream, rec_info, outbuf, rec_info._header_length ) ;
  }

  //--------------------------------------------------------------------------

  inline std::pair<record_info, buffer> api::read_record( sio::ifstream &stream ) {
    record_info rec_info ;
    buffer outbuf( sio::mbyte ) ;
    api::read_record( stream, rec_info, outbuf ) ;
    return std::make_pair( rec_info, std::move( outbuf ) ) ;
  }

  //--------------------------------------------------------------------------

  template <class UnaryPredicate>
  inline void api::skip_records( sio::ifstream &stream, UnaryPredicate pred ) {
    sio::record_info rec_info ;
    sio::buffer rec_buffer( sio::max_record_info_len ) ;
    while( 1 ) {
      // read record header
      api::read_record_info( stream, rec_info, rec_buffer ) ;
      // skip record data
      stream.seekg( rec_info._file_end ) ;
      if( not stream.good() ) {
        SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after a seek operation!" ) ;
      }
      if( not pred( rec_info ) ) {
        break ;
      }
    }
  }

  inline void api::skip_records( sio::ifstream &stream, std::size_t nskip ) {
    std::size_t counter = 0 ;
    api::skip_records( stream, [&]( const record_info & ) {
      ++ counter ;
      return ( counter < nskip ) ;
    }) ;
  }

  //--------------------------------------------------------------------------

  inline void api::skip_records( sio::ifstream &stream, std::size_t nskip, const std::string &name ) {
    std::size_t counter = 0 ;
    api::skip_records( stream, [&]( const record_info &rec_info ) {
      if( name == rec_info._name ) {
        ++ counter ;  
      }
      return ( counter < nskip ) ;
    }) ;
  }

  //--------------------------------------------------------------------------

  inline void api::go_to_record( sio::ifstream &stream, const std::string &name ) {
    record_info goto_info ;
    api::skip_records( stream, [&]( const record_info &rec_info ) {
      if( name == rec_info._name ) {
        goto_info = rec_info ;
        return false ;
      }
      return true ;
    }) ;
    stream.seekg( goto_info._file_start ) ;
    if( not stream.good() ) {
      SIO_THROW( sio::error_code::bad_state, "ifstream is in a bad state after a seek operation!" ) ;
    }
  }

}