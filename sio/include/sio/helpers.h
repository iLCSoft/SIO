#pragma once

// -- sio headers
#include <sio/definitions.h>

namespace sio {

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
