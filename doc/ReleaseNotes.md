# v00-00-04

* 2020-11-23 Marko Petric ([PR#12](https://github.com/iLCSoft/SIO/pull/12))
  - Setup rpath for macOS and give optional flag for turning on rpath on other systems
  - Resolve correctly linker independently of OS and pass flags

* 2020-11-19 Remi Ete ([PR#11](https://github.com/iLCSoft/SIO/pull/11))
  - Fix clang warnings

# v00-00-03

* 2020-10-28 Frank Gaede ([PR#4](https://github.com/ilcsoft/sio/pull/4))
  - set the default compression level to Z_DEFAULT_COMPRESSION (-1)
         - correct the documentation for `set_level()`

* 2020-09-30 tmadlener ([PR#5](https://github.com/ilcsoft/sio/pull/5))
  - Remove `sio-dump-detailed` and add a `--detailed` / `-d` flag to `sio-dump`. Slightly improve the output in case an error occurs.

* 2020-09-28 Remi Ete ([PR#8](https://github.com/ilcsoft/sio/pull/8))
  - Examples:
      - Turned example structure linked_list to normal pointer
      - Fixed buffer overflow issue
  - **Important for documentation: pointer relocation using smart pointers** (`std::shared_ptr<T>` and co) **is not supported**

# v00-00-02

* 2019-09-30 Remi Ete ([PR#2](https://github.com/iLCSoft/SIO/pull/2))
  - Added `buffer_span::dump` for debugging
  - More debug printout
  - CMake: Reviewed way of setting the CXX standard

* 2019-07-13 Remi Ete ([PR#1](https://github.com/iLCSoft/SIO/pull/1))
  - Added buffer::shrink method to optimize memory

# v00-00-01

- first version of standalone SIO package. Shipped out from LCIO and re-worked