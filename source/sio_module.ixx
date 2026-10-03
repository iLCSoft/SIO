// SPDX-License-Identifier: GPL-3.0-or-later
// Module interface for SIO - Simple I/O library
//
// This module exports the core SIO types and interfaces.

module;

// Global module fragment - includes go here
#include <cctype>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

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

// SIO headers
#include "sio/definitions.h"
#include "sio/exception.h"
#include "sio/version.h"
#include "sio/buffer.h"
#include "sio/block.h"
#include "sio/io_device.h"
#include "sio/memcpy.h"
#include "sio/api.h"
#include "sio/compression/zlib.h"

export module sio;

// Export SIO namespace and types
export namespace sio {
  // Type aliases from definitions.h
  using sio::byte;
  using sio::byte_array;
  using sio::byte_traits;
  using sio::index_type;
  using sio::options_type;
  using sio::version_type;
  using sio::ptr_type;
  using sio::pointed_at_map;
  using sio::pointer_to_map;
  using sio::ifstream;
  using sio::ofstream;
  using sio::fstream;
  using sio::block_ptr;
  using sio::block_list;

  // Constants from definitions.h
  using sio::null_byte;
  using sio::padding_bytes;
  using sio::kbyte;
  using sio::mbyte;
  using sio::compression_bit;
  using sio::bit_align;
  using sio::padding;
  using sio::padding_mask;
  using sio::record_marker;
  using sio::block_marker;
  using sio::max_record_name_len;
  using sio::max_record_info_len;
  using sio::single_len;
  using sio::double_len;
  using sio::quad_len;
  using sio::octo_len;

  // Structs and enums from definitions.h
  using sio::record_info;
  using sio::validate;
  using sio::valid_record_name;

  // Exception types
  using sio::exception;
  using sio::error_code;

  // Version class
  using sio::version;

  // Core classes
  using sio::buffer;
  using sio::buffer_span;
  using sio::block;
  using sio::read_device;
  using sio::write_device;

  // Memory copy utilities
  using sio::memcpy;
  using sio::sizeof_helper;

  // API class and functions
  using sio::api;

  // Compression
  using sio::zlib_compression;
}
