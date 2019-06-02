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
