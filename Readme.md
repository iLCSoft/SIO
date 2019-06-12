# SIO - Simple IO

[![Build Status](https://travis-ci.org/iLCSoft/SIO.svg?branch=master)](https://travis-ci.org/iLCSoft/SIO)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/11178/badge.svg)](https://scan.coverity.com/projects/ilcsoft-sio)

SIO is a persistency solution for reading and writing binary data in SIO structures called *record* and *block*. 
SIO has originally been implemented as persistency layer for [LCIO](https://github.com/iLCSoft/LCIO).

## Installation

```shell
# Download it
$ git clone https://github.com/iLCSoft/SIO.git
$ cd SIO
# compile the software
$ mkdir build
$ cd build
$ cmake ..
$ make install
```

Options can be given to CMake:

- INSTALL_DOC (ON/OFF): to generate and install C++ API documentation using Doxygen
- SIO_EXAMPLES (ON/OFF): to compile SIO examples
- SIO_LOGLVL (0-5): The log level internally used by SIO. 0 means SILENT and 5 or more means DEBUG. This is a developer feature, don't use it!

## Documentation

The examples provided in source/examples serve as documentation in addition to the C++ API documentation generated when configuring the software with `-DINSTALL_DOC=ON`.

## Copyright and Licence

Copyright (c) 2003-2016, DESY, Deutsches Elektronen Synchrotron

Copyright (c) 2003-2016, SLAC, National Accelerator Laboratory

SIO is distributed under the [BSD-3-Clause licence](http://opensource.org/licenses/BSD-3-Clause).



