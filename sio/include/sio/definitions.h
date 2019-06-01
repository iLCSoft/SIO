#pragma once

// -- std headers
#include <iostream>
#include <vector>
#include <type_traits>
#ifdef __APPLE__
#include <_types.h>
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#include <_types/_uint8_t.h>
#include <sys/_types/_int16_t.h>
#include <sys/_types/_int64_t.h>
#else
#include <stdint.h>
#endif

// ----------------------------------------------------------------------------
// Deal with 'endian-ness'.  Try to base this on the processor type (because
// the operating system doesn't guarantee endian-ness ... Linux runs happily
// on both x86 CPUs (little endian) and PPC CPUs (big endian)).
//
// Branch on flag provided by compiler:
//
// OS            CPU           Macro         Provided by   Endian-ness
// ------------  ------------  ------------  ------------  -----------
// AIX           PPC(?)        _AIX          GNU compiler  Big
// OSF1          Alpha         __alpha__     GNU compiler  Little
// Linux         x86           __i386__      GNU compiler  Little
// Linux         Opteron       _LP64         GNU compiler  Little
// Linux         itanium       _LP64         GNU compiler  Little
// SunOS         Sparc         __sparc__     GNU compiler  Big
// Windows/NT    Alpha         _M_ALPHA      VC  compiler  Little
// Windows/NT    x86           _M_IX86       VC  compiler  Little
// Windows/NT    MIPS          _M_MRX000     VC  compiler  ?
// Windows/NT    PPC           _M_PPC        VC  compiler  Big
// ----------------------------------------------------------------------------
#if defined(__alpha__) ||   defined(__i386__)  ||   defined(_M_ALPHA)  ||   defined(_M_IX86) || defined(_LP64) || defined(__LITTLE_ENDIAN__)
#define SIO_LITTLE_ENDIAN
#endif

#if defined(_AIX)      ||   defined(__sparc__) ||   defined(_M_PPC) || ( defined(__APPLE_CC__) && !defined(__LITTLE_ENDIAN__)  ) 
#define SIO_BIG_ENDIAN
#endif

#if !defined(SIO_LITTLE_ENDIAN) && !defined(SIO_BIG_ENDIAN)
#error "Couldn't determine endianess on this plateform!"
#endif

namespace sio {
  
  // ----------------------------------------------------------------------------
  // Deal with pointer length.  Currently, the only problem is alpha which uses
  // 64 bit pointers.                                                          
  //                                                              
  // OS            CPU           Macro         Provided by   Pointer size
  // ------------  ------------  ------------  ------------  -----------
  // AIX           PPC(?)        _AIX          GNU compiler  4 bytes
  // OSF1          Alpha         __alpha__     GNU compiler  8 bytes
  // Linux         x86           __i386__      GNU compiler  4 bytes
  // Linux         Opteron       _LP64    _    GNU compiler  8 bytes
  // Linux         Itanium       _LP64         GNU compiler  8 bytes
  // SunOS         Sparc         __sparc__     GNU compiler  4 bytes
  // Windows/NT    Alpha         _M_ALPHA      VC  compiler  8 bytes
  // Windows/NT    x86           _M_IX86       VC  compiler  4 bytes
  // Windows/NT    MIPS          _M_MRX000     VC  compiler  ? bytes
  // Windows/NT    PPC           _M_PPC        VC  compiler  4 bytes
  // ----------------------------------------------------------------------------
  // Old check was problematic because both macros could evaluate to true, and it would
  // also be possible that sio::ptr_type was not defined at all.  Change as needed.
  // --JM 
#if defined(__alpha__) || defined(_M_ALPHA) || defined(_LP64)
  using ptr_type = std::size_t ;
#else
  using ptr_type = unsigned int ;
#endif

  ///< Bytes related types
  using byte = unsigned char ;
  using byte_array = std::vector<unsigned char> ;
  using byte_traits = std::char_traits<byte> ;
  using index_type = std::size_t ;
  using options_type = unsigned int ;
  
  ///< Kilo byte unit
  static constexpr std::size_t kbyte = 0x00000400 ;
  ///< Mega byte unit
  static constexpr std::size_t mbyte = 0x00100000 ;
  ///< The compression bit mask
  static constexpr unsigned int compression_bit = 0x00000001 ;
  ///< The bit alignment mask
  static constexpr unsigned int bit_align = 0x00000003 ;
  ///< The additional padding added in buffer IO
  static constexpr unsigned int padding = 3 ;
  ///< The additional padding mask
  static constexpr unsigned int padding_mask = 0xfffffffc ;
  ///< The record marker
  static constexpr unsigned int record_marker = 0xabadcafe ;
  ///< The block marker
  static constexpr unsigned int block_marker  = 0xdeadbeef ;
  
  static constexpr std::size_t max_record_name_len = 64 ;
  
  static constexpr byte null_byte = '\0' ;
  
  // TODO: Do we still need all of this ??
  static constexpr std::size_t single_len = 1 ;
  static constexpr std::size_t double_len = 2 ;  
  static constexpr std::size_t quad_len = 4 ;
  static constexpr std::size_t octo_len = 8 ;
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  /**
   *  @brief  version_helper class.
   *
   *  Helper class to encode or decode version in a single variable
   */
  class version_helper {
  public:
    using version_type   = uint32_t ;
    using major_type     = uint16_t ;
    using minor_type     = uint16_t ;
  
  public:  
    // static API only
    version_helper() = delete ;
    
    /**
     *  @brief  Encode a version from a major and minor version number 
     *  
     *  @param  major the major version number
     *  @param  minor the minor version number
     */
    static inline version_type encode( major_type major, minor_type minor ) noexcept {
      return (((major) << 16) + (minor)) ;
    }
    
    /**
     *  @brief  Decode a minor version number from the version number 
     *  
     *  @param  version the full version number
     */
    static inline minor_type minor( version_type version ) noexcept {
      return ((version) & 0x0000ffff) ;
    }
    
    /**
     *  @brief  Decode a major version number from the version number 
     *  
     *  @param  version the full version number
     */
    static inline major_type major( version_type version ) noexcept {
      return (((version) & 0xffff0000) >> 16) ;
    }
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  /**
   *  @brief  memcpy_helper class
   *
   *  Perform raw data copy and deals with plateform endian-ness
   */
  class memcpy_helper {
  public:
    // static API only
    memcpy_helper() = delete ;
    
    /**
     *  @brief  Perform a reverse byte copy
     *  
     *  @param  from the input bytes address to copy
     *  @param  dest the output destination of copied bytes
     *  @param  size the size of the element in the bytes
     *  @param  count the number of elements to copy
     */
    static void reverse_copy( const byte *const from, byte *dest, std::size_t size, std::size_t count ) {
      dest += size;
      std::size_t jump = size << 1;
      const byte *lfrom = from ;
      for( std::size_t icnt = 0; icnt < count; icnt++ ) {
        for( std::size_t ibyt = 0; ibyt < size; ibyt++  ) {
          *--dest = *lfrom++;
        }
        dest += jump;
      }
    }
    
    /**
     *  @brief  Perform a byte array copy
     *  
     *  @param  from the input bytes address to copy
     *  @param  dest the output destination of copied bytes
     *  @param  size the size of the element in the bytes
     *  @param  count the number of elements to copy
     */
    static void copy( const byte *const from, byte *dest, std::size_t size, std::size_t count ) {
#ifdef SIO_BIG_ENDIAN
      memcpy( dest, from, size * count ) ;
#else
      memcpy_helper::reverse_copy( from, dest, size, count ) ;
#endif
    }
  };
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  
  /**
   *  @brief  string_helper class
   *
   *  Collection of static helper method related to strings
   */
  class string_helper {
  public:
    // static API only
    string_helper() = delete ;
    
    /**
     *  @brief  Validate a name.
     *  
     *  SIO only accepts names starting with (regular expression) [A-Za-z_] 
     *  and continuing with [A-Za-z0-9_] (which most people will recognize 
     *  as the definition of a C/C++ variable name).
     *  
     *  @param  name the strin name to test
     */
    static inline bool validate( const std::string &name ) {
      auto cname = name.c_str() ;
      if( *cname < 0 ) {
        return false;
      }
      if( !isalpha( (int)*cname ) && *cname != '_' ) {
        return false;
      }
      for( cname += 1; *cname != '\0'; cname++ ) {
        if( *cname < 0 ) {
          return false;
        }
        if( !isalnum( (int)*cname ) && *cname != '_' ) {
          return false;
        }
      }
      return true;
    }
    
    /**
     *  @brief  Validate a record name
     *  
     *  @param  name the record name to validate
     */
    static inline bool valid_record_name( const std::string &name ) {
      if( not string_helper::validate( name ) ) {
        return false ;
      }
      if( name.size() > sio::max_record_name_len ) {
        return false ;
      }
      return true ;
    }
  };

}

// SIO_LOGLVL defines the log level. The verbosity is fixed 
// at compile time to avoid performance issue due to logging
// Log levels:
// - silent: 0
// - debug: 1
// - info: 2
// - warning: 3
// - error: 4
#ifndef SIO_LOGLVL
#define SIO_LOGLVL 2
#endif

#if SIO_LOGLVL > 0
#define SIO_DEBUG( message ) std::cout << "[SIO DEBUG] - " << message << std::endl
#else
#define SIO_DEBUG( message )
#endif

#if SIO_LOGLVL > 1
#define SIO_INFO( message ) std::cout << "[SIO INFO] - " << message << std::endl
#else
#define SIO_INFO( message )
#endif

#if SIO_LOGLVL > 2
#define SIO_WARNING( message ) std::cout << "[SIO WARNING] - " << message << std::endl
#else
#define SIO_WARNING( message )
#endif

#if SIO_LOGLVL > 3
#define SIO_ERROR( message ) std::cout << "[SIO ERROR] - " << message << std::endl
#else
#define SIO_ERROR( message )
#endif

#define SIO_BYTE_CAST(pntr)    (reinterpret_cast<sio::byte*>((pntr)))
#define SIO_CBYTE_CAST(pntr)    (reinterpret_cast<const sio::byte*>((pntr)))







