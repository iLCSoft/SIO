#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {

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
    static version_type encode_version( major_type major, minor_type minor ) noexcept ;

    /**
     *  @brief  Decode a minor version number from the version number
     *
     *  @param  version the full version number
     */
    static minor_type minor_version( version_type version ) noexcept ;

    /**
     *  @brief  Decode a major version number from the version number
     *
     *  @param  version the full version number
     */
    static major_type major_version( version_type version ) noexcept ;
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
    static void reverse_copy( const unsigned char *const from, unsigned char *dest, std::size_t size, std::size_t count ) ;

    /**
     *  @brief  Perform a byte array copy
     *
     *  @param  from the input bytes address to copy
     *  @param  dest the output destination of copied bytes
     *  @param  size the size of the element in the bytes
     *  @param  count the number of elements to copy
     */
    static void copy( const unsigned char *const from, unsigned char *dest, std::size_t size, std::size_t count ) ;
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
    static bool validate( const std::string &name ) ;

    /**
     *  @brief  Validate a record name
     *
     *  @param  name the record name to validate
     */
    static bool valid_record_name( const std::string &name ) ;
  };

}

#include <sio/details/helpers_impl.h>
