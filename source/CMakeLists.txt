#################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
# @author Remi Ete, DESY
#################################

# SIO care about endianess
INCLUDE( TestBigEndian )
TEST_BIG_ENDIAN( BIG_ENDIAN )

FILE( GLOB_RECURSE SIO_SRCS src/*.cc )

# build the SIO library
SIO_ADD_SHARED_LIBRARY( sio ${SIO_SRCS} )
ADD_LIBRARY(SIO::sio ALIAS sio)
TARGET_LINK_LIBRARIES( sio PRIVATE ZLIB::ZLIB )
TARGET_INCLUDE_DIRECTORIES( sio PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
TARGET_COMPILE_DEFINITIONS(sio PUBLIC "-DSIO_LOGLVL=${SIO_LOGLVL}")
IF( NOT BIG_ENDIAN )
  TARGET_COMPILE_DEFINITIONS(sio PUBLIC "-D__LITTLE_ENDIAN__")
ENDIF()
IF( SIO_MACROS_WITH_EXCEPTION )
  TARGET_COMPILE_DEFINITIONS(sio PUBLIC "-DSIO_MACROS_WITH_EXCEPTION=1")
ENDIF()

# Add C++20 module interface if enabled (before install)
IF( SIO_ENABLE_CXX_MODULES )
  SIO_ADD_MODULE_INTERFACE( sio ${CMAKE_CURRENT_SOURCE_DIR}/sio_module.ixx )
  # Install library with module file set
  INSTALL( TARGETS sio
    EXPORT SIOTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sio/modules
    PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
  )
ELSE()
  # Install library without modules
  SIO_INSTALL_SHARED_LIBRARY( sio
    EXPORT SIOTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR} )
ENDIF()

# install SIO headers
SIO_INSTALL_DIRECTORY( include/sio DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

# build dump binaries
ADD_EXECUTABLE( sio-dump main/sio-dump.cc )
TARGET_LINK_LIBRARIES( sio-dump sio )
INSTALL( TARGETS sio-dump
  EXPORT SIOTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} )

# SIO examples
IF( SIO_EXAMPLES )
  ADD_SUBDIRECTORY( examples )
  
  ADD_TEST( t_simple_write "${EXECUTABLE_OUTPUT_PATH}/simple_write" simple.sio )
  SET_TESTS_PROPERTIES( t_simple_write PROPERTIES PASS_REGULAR_EXPRESSION "Written sio file simple.sio" )
  
  ADD_TEST( t_simple_read "${EXECUTABLE_OUTPUT_PATH}/simple_read" simple.sio )
  SET_TESTS_PROPERTIES( t_simple_read PROPERTIES PASS_REGULAR_EXPRESSION "Read sio file simple.sio" )
  SET_TESTS_PROPERTIES( t_simple_read PROPERTIES DEPENDS "t_simple_write" )
  
  ADD_TEST( t_zlib_write "${EXECUTABLE_OUTPUT_PATH}/zlib_write" zlib.sio )
  SET_TESTS_PROPERTIES( t_zlib_write PROPERTIES PASS_REGULAR_EXPRESSION "Written sio file zlib.sio" )
  
  ADD_TEST( t_zlib_read "${EXECUTABLE_OUTPUT_PATH}/zlib_read" zlib.sio )
  SET_TESTS_PROPERTIES( t_zlib_read PROPERTIES PASS_REGULAR_EXPRESSION "Read sio file zlib.sio" )
  SET_TESTS_PROPERTIES( t_zlib_read PROPERTIES DEPENDS "t_zlib_write" )
  
  ADD_TEST( t_relocation_write "${EXECUTABLE_OUTPUT_PATH}/relocation_write" relocation.sio )
  SET_TESTS_PROPERTIES( t_relocation_write PROPERTIES PASS_REGULAR_EXPRESSION "Written sio file relocation.sio" )
  
  ADD_TEST( t_relocation_read "${EXECUTABLE_OUTPUT_PATH}/relocation_read" relocation.sio )
  SET_TESTS_PROPERTIES( t_relocation_read PROPERTIES PASS_REGULAR_EXPRESSION "Read sio file relocation.sio with 200 elements" )
  SET_TESTS_PROPERTIES( t_relocation_read PROPERTIES DEPENDS "t_relocation_write" )
ENDIF()
