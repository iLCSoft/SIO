// -- sio headers
#include <sio/definitions.h>
#include <sio/api.h>
#include <sio/exception.h>
// -- std headers
#include <limits>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <stdexcept>



constexpr const char *USAGE = R"(Usage: sio-dump [-d/--detailed] [-n NRECORDS] [-m SKIPRECORDS] siofile)";

constexpr const char *HELP = R"(Dump SIO record from file to console

Positional arguments:
  siofile:         The file from which the records should be dumped

Optional arguments:
  -h, --help       Show the help message and exit
  -d, --detailed   Dump detailed (i.e. unpacked) records from the file
  -n NRECORDS      Only dump NRECORDS records from the file
  -m SKIPRECORDS   Skip the first SKIPRECORDS from dumping
)";

/**
 * @brief Check if either a short form or a long form of the option is in the
 * list of arguments.
 */
bool has_option(std::vector<std::string>& args, const char* opt_s, const char* opt_l) {
  const auto it = std::find_if(args.cbegin(), args.cend(),
                               [opt_s, opt_l] (const std::string& arg) {
                                 return arg == opt_s || arg == opt_l;
                               });

   if (it != args.cend()) {
     args.erase(it);
     return true;
   }

   return false;
}

/**
 * @brief Check if a given option is in the list of arguments and get the
 * corresponding value or a default value if the argument is not present.
 *
 * Print the usage message and exit if the conversion fails or if the value to
 * the argument is missing.
 */
int option_val(std::vector<std::string>& args, const char* opt_s, int def_val=-1) {
  const auto it = std::find_if(args.cbegin(), args.cend(), [opt_s] (const std::string& arg) {
    return arg == opt_s;
  });

  if (it != args.cend()) {
    const auto value_index = std::distance(args.cbegin(), it) + 1;
    // make sure that the value follows the argument
    if ((int)args.size() < value_index) { // silence the Wsign-compare
      std::cout << USAGE << std::endl;
      std::exit(1);
    }
    try {
      const int value = std::stoi(args[value_index]);
      args.erase(it, it + 1);
      return value;
    } catch (const std::invalid_argument&) {
      std::cerr << "Cannot convert \'" << args[value_index] << "\' to int for argument \'" << opt_s << "\'\n\n";
      std::cout << USAGE << std::endl;
      std::exit(1);
    }
  }

  return def_val;
}

/**
 *  @brief  Utility in sio to dump all records from a file on disk
 */
int main( int argc, char **argv ) {
  std::vector<std::string> args(argv + 1, argv + argc);

  const int count = option_val(args, "-n", std::numeric_limits<int>::max());
  const int skip = option_val(args, "-m", 0);
  const bool detailed = has_option(args, "-d", "--detailed");

  if (has_option(args, "-h", "--help")) {
    std::cout << USAGE << "\n\n";
    std::cout << HELP << std::endl;
    return 0;
  }

  if (args.size() < 1) {
    std::cout << USAGE << std::endl;
    return 0;
  }

  const auto& fname = args.back();

  sio::ifstream file;
  file.open( fname , std::ios::in | std::ios::binary ) ;
  try {
    sio::api::dump_records( file, skip, count, detailed) ;
  } catch( const sio::exception &e ) {
    switch( e.code() ) {
      case sio::error_code::not_open:
        std::cerr << "Cannot open file \'" << fname << "\'" << std::endl;
        break;
      case sio::error_code::no_marker:
        std::cerr << "Interrupt: no record marker found\n";
        break;
      default:
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
  }

  return 0 ;
}

