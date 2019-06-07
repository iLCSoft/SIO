#pragma once

// -- sio headers
#include <sio/definitions.h>

// -- std headers
#include <memory>
#include <utility>
#include <string>
#include <vector>

namespace sio {
  
  class buffer ;
  class block ;
  class write_device ;

  /**
   *  @brief  api class.
   *  Helper class to perform high level operation on sio objects
   */
  class api {
  public:
    // static API only
    api() = delete ;
    
    /**
     *  @name Pointer relocation
     */
    ///@{
    /**
     *  @brief  Perform the pointer relocation after the record has been read.
     *  
     *  @param  pointed_at the map of pointers "pointed at"
     *  @param  pointer_to the map of pointers "pointer to"
     */
    static void read_relocation( pointed_at_map& pointed_at, pointer_to_map& pointer_to ) ;
    
    /**
     *  @brief  Perform the pointer relocation after the record has been written.
     *          This operation requires to know the beginning of the record buffer 
     *          address to compute the address shift
     *
     *  @param  rec_start the address of the start of the record
     *  @param  pointed_at the map of pointers "pointed at"
     *  @param  pointer_to the map of pointers "pointer to"
     */
    static void write_relocation( const sio::byte* rec_start, pointed_at_map& pointed_at, pointer_to_map& pointer_to ) ;
    ///@}

    /**
     *  @name Buffer I/O
     */
    ///@{
    /**
     *  @brief  Read data from the buffer. The template form allows
     *  for using either a sio::buffer or a sio::buffer_span object
     *
     *  @param  buffer the buffer to read from
     *  @param  ptr the address of the variable to receive
     *  @param  position the position in the buffer
     *  @param  count the number of bytes to read
     *  @return the actual number of bytes read out
     */
    template <class bufT, typename T>
    static typename bufT::size_type read( const bufT &buffer, T *ptr, typename bufT::index_type position, typename bufT::size_type count ) ;

    /**
     *  @brief  Read data from the buffer. The template form allows
     *  for using either a sio::buffer or a sio::buffer_span object
     *
     *  @param  buffer the buffer to read from
     *  @param  ptr the address of the variable to receive
     *  @param  length the length of the variable
     *  @param  position the position in the buffer
     *  @param  count the number of bytes to read
     *  @return the actual number of bytes read out
     */
    template <class bufT>
    static typename bufT::size_type read( const bufT &buffer, typename bufT::pointer ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) ;

    /**
     *  @brief  Write data to the buffer. The template form allows
     *  for using either a sio::buffer or a sio::buffer_span object
     *
     *  @param  buffer the buffer to write to
     *  @param  ptr the address of the variable to write
     *  @param  position the position in the buffer
     *  @param  count the number of bytes to write
     *  @return the actual number of bytes written out
     */
    template <class bufT, typename T>
    static typename bufT::size_type write( bufT &buffer, const T *const ptr, typename bufT::index_type position, typename bufT::size_type count ) ;

    /**
     *  @brief  Write data to the buffer. The template form allows
     *  for using either a sio::buffer or a sio::buffer_span object
     *
     *  @param  buffer the buffer to write to
     *  @param  ptr the address of the variable to write
     *  @param  length the length of the variable
     *  @param  position the position in the buffer
     *  @param  count the number of bytes to write
     *  @return the actual number of bytes written out
     */
    template <class bufT>
    static typename bufT::size_type write( bufT &buffer, typename bufT::const_pointer const ptr, typename bufT::size_type length, typename bufT::index_type position, typename bufT::size_type count ) ;
    ///@}

    /**
     *  @name Record I/O
     */
    ///@{
    /**
     *  @brief  Read the next record header from the input stream.
     *          On exit, the stream cursor is set after the record header, ready
     *          to read the incoming record data bytes. The record header bytes 
     *          read out from the stream are stored in the buffer passed as third 
     *          argument. If the buffer is not large enough to contain all bytes,
     *          it is expanded.  
     *  
     *  @param  stream the input stream
     *  @param  rec_info the record info to receive
     *  @param  outbuf the buffer containing the record info bytes
     */
    static void read_record_info( sio::ifstream &stream, record_info &rec_info, buffer &outbuf ) ;
    
    /**
     *  @brief  Read out the record data from the input stream. The record data 
     *          bytes are written in the buffer passed by reference. By default, the 
     *          bytes are written at the beginning of the buffer. The last argument
     *          allows for specifying a shift from the start of the buffer. 
     *          
     *  @param  stream the input stream
     *  @param  rec_info the record info
     *  @param  outbuf the buffer to receive the record data bytes
     *  @param  buffer_shift an optional shift from the start of the buffer
     */
    static void read_record_data( sio::ifstream &stream, const record_info &rec_info, buffer &outbuf, std::size_t buffer_shift = 0 ) ;
    
    /**
     *  @brief  Read out the record (header + data) from the input stream.
     *          Simple combination of the functions above.
     *   
     *  @param  stream the input stream
     *  @param  rec_info the record info to receive
     *  @param  outbuf the record header + data bytes to receive
     */
    static void read_record( sio::ifstream &stream, record_info &rec_info, buffer &outbuf ) ;
    
    /**
     *  @brief  Read out the record (header + data) from the input stream.
     *          Simple combination of the functions above. Returns the record
     *          info and the buffer. The initial buffer size is set to 1 Mo and
     *          might be expanded while reading out the record data if required.
     *          
     *  @param  stream the input stream
     */
    static std::pair<record_info, buffer> read_record( sio::ifstream &stream ) ;
    
    /**
     *  @brief  Skip the next records while the unary predicate is true.
     *          The predicate must be of the following form :
     *          'bool predicate(const record_info&)'.
     *          
     *  @param  stream the input stream
     *  @param  pred the unary predicate
     */
    template <class UnaryPredicate>
    static void skip_records( sio::ifstream &stream, UnaryPredicate pred ) ;
    
    /**
     *  @brief  Skip the N next records from the input stream
     *  
     *  @param  stream the input stream
     *  @param  nskip the number of record to skip
     */
    static void skip_records( sio::ifstream &stream, std::size_t nskip ) ;
    
    /**
     *  @brief  Skip the N next records with a specific name.
     *          If a record with a different name is encountered, it is also skipped.
     *           
     *  @param  stream the input stream
     *  @param  nskip the number of record to skip
     *  @param  name the record name to skip (only)
     */
    static void skip_records( sio::ifstream &stream, std::size_t nskip, const std::string &name ) ;
    
    /**
     *  @brief  Got to the next record with the spcified name
     *  
     *  @param  stream the input stream
     *  @param  name the target record name
     */
    static void go_to_record( sio::ifstream &stream, const std::string &name ) ;
    
    /**
     *  @brief  Extract all the block info from the buffer. Skip block reading
     *  
     *  @param  buf the input block buffer
     */
    static std::vector<block_info> read_block_infos( const buffer_span &buf ) ;
    
    /**
     *  @brief  Extract the block info and get a buffer span of the block at the given index
     *  
     *  @param  rec_buf the record buffer
     *  @param  index the index of block header start in the record buffer
     */
    static std::pair<block_info, buffer_span> extract_block( const buffer_span &rec_buf, buffer_span::index_type index ) ;
    
    /**
     *  @brief  Decode the record buffer using the block decoder.
     *          Loop over the blocks found in the buffer and try to decode it.
     *          If the block decoder is not available, it is skipped
     *  
     *  @param  rec_buf the record buffer pointing on the first block to decode
     *  @param  blocks the list of block decoder to use 
     */
    static void read_blocks( const buffer_span &rec_buf, const block_list &blocks ) ;
    
    /**
     *  @brief  Dump the records from the input stream to the console.
     *          Note that if you use a detailed printout, the record 
     *          is first uncompressed using zlib and the block infos 
     *          are printed out too.
     *  
     *  @param  stream the input stream
     *  @param  skip the number of records to skip from the current position
     *  @param  count the number of record to printout
     *  @param  detailed whether to printout detailed information (block info)
     */
    static void dump_records( sio::ifstream &stream, std::size_t skip, std::size_t count, bool detailed ) ;
    
    /**
     *  @brief  Write the blocks in the buffer contained in the write_device.
     *          For each block, a block header and the block data is written
     *  
     *  @param  device the write device to write to
     *  @param  blocks the block encoder
     */
    static void write_blocks( write_device &device, const block_list &blocks ) ;

    /**
     *  @brief  Write a record in a buffer. A record_info object is returned. 
     *          Note that in the returned structure the fields _file_start and
     *          _file_end are not filled since the writting is only done in the
     *          buffer at this step. Note also that this function doesn't call
     *          any compression algorithm. See overloads to get a compressed 
     *          buffer.
     *  
     *  @param  name the record name
     *  @param  rec_buf the record buffer to receive
     *  @param  blocks the block list for writting
     *  @param  opts the record options
     */
    static record_info write_record( const std::string &name, buffer &rec_buf, const block_list& blocks, sio::options_type opts ) ;
    
    /**
     *  @brief  Compress the record buffer. Note that several operation are done 
     *          in this function:
     *          - the record buffer is compressed and receive in the comp_buf
     *          - the record info is updated with the compressed record data length
     *          - the record header is overwritten in the record buffer 
     *          
     *  @param  rec_info the record info instance
     *  @param  rec_buf the record buffer
     *  @param  comp_buf the compressed buffer to receive
     */
    template <typename compT>
    static void compress_record( record_info &rec_info, buffer &rec_buf, buffer &comp_buf, compT &compressor ) ;
    
    /**
     *  @brief  Write the full record buffer in the output stream. The stream 
     *          is flushed after writing the buffer
     *  
     *  @param  stream the output stream
     *  @param  rec_buf the full record buffer (header + data)
     *  @param  rec_info the record info to update (file start and end positions)
     */
    static void write_record( sio::ofstream &stream, const buffer_span &rec_buf, record_info &rec_info ) ;
    
    /**
     *  @brief  Write the record buffer in two step from two buffers. The first 
     *          buffer contains the record header buffer and the second the 
     *          record data, either compressed or uncompressed. The stream is 
     *          flushed after writing the two buffers
     *          
     *  @param  stream the output stream
     *  @param  hdr_span the record header buffer span
     *  @param  data_span the record data buffer span
     *  @param  rec_info the record info to update (file start and end positions)
     */
    static void write_record( sio::ofstream &stream, const buffer_span &hdr_span, const buffer_span &data_span, record_info &rec_info ) ;
    ///@}

    /**
     *  @name Compression
     */
    ///@{
    /**
     *  @brief  Extract the compression bit from the option word
     * 
     *  @param  opts the options word
     */
    static bool is_compressed( options_type opts ) ;
    
    /**
     *  @brief  Turn on/off the compression bit in the options word
     *  
     *  @param  opts the option word
     *  @param  value whether to set on/off the compression bit
     *  @return the old compression bit value
     */
    static bool set_compression( options_type &opts, bool value ) ;
    ///@}
  };
  
}

#include <sio/details/api_impl.h>
