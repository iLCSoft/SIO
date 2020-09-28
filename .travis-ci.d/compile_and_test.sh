#!/bin/bash
yum install valgrind -y

ILCSOFT=/cvmfs/clicdp.cern.ch/iLCSoft/builds/current/CI_${COMPILER}
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -GNinja -DSIO_BUILTIN_ZLIB=$BUILTIN_ZLIB -DSIO_EXAMPLES=ON -DSIO_SANITIZERS=address,leak,undefined -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
ninja  -k 0 && \
ninja install 
#&& \
#ctest --output-on-failure --verbose

./bin/relocation_write
/usr/bin/valgrind ./bin/relocation_read relocation.sio