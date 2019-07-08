#################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
# @author Remi Ete, DESY
#################################

# SIO care about endianess
INCLUDE( TestBigEndian )
TEST_BIG_ENDIAN( BIG_ENDIAN )

IF( NOT BIG_ENDIAN )
    ADD_DEFINITIONS( "-D__LITTLE_ENDIAN__" )
ENDIF()

INCLUDE_DIRECTORIES( include tests )
FILE( GLOB_RECURSE SIO_SRCS src/*.cc )

ADD_DEFINITIONS( "-DSIO_LOGLVL=${SIO_LOGLVL}" )
IF( SIO_MACROS_WITH_EXCEPTION )
  ADD_DEFINITIONS( "-DSIO_MACROS_WITH_EXCEPTION=1" )
ENDIF()

# build the SIO library
SIO_ADD_SHARED_LIBRARY( sio ${SIO_SRCS} )
TARGET_LINK_LIBRARIES( sio ${ZLIB_LIBRARIES} )
SIO_INSTALL_SHARED_LIBRARY( sio DESTINATION lib )

# install SIO headers
SIO_INSTALL_DIRECTORY( include DESTINATION . )

# build dump binaries
ADD_EXECUTABLE( sio-dump main/sio-dump.cc )
TARGET_LINK_LIBRARIES( sio-dump sio )
INSTALL( TARGETS sio-dump RUNTIME DESTINATION bin )

ADD_EXECUTABLE( sio-dump-detailed main/sio-dump-detailed.cc )
TARGET_LINK_LIBRARIES( sio-dump-detailed sio )
INSTALL( TARGETS sio-dump-detailed RUNTIME DESTINATION bin )

# add unit tests
SIO_ADD_TEST_REG (
  test_exception
  BUILD_EXEC
  REGEX_FAIL "TEST_FAILED"
)

# SIO examples
IF( SIO_EXAMPLES )
  ADD_SUBDIRECTORY( examples )
ENDIF()
