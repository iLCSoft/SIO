#!/bin/bash

source /cvmfs/sft.cern.ch/lcg/views/${VIEW}/setup.sh

cd /Package
mkdir build install
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_CXX_STANDARD=${STANDARD} -DSIO_BUILTIN_ZLIB=${BUILTIN_ZLIB} -DCMAKE_CXX_FLAGS=" -fdiagnostics-color=always " -G Ninja .. && \
ninja -k0 && \
ninja install && \
ctest --output-on-failure
