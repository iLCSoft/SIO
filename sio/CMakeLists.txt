#################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
# @author Remi Ete, DESY
#################################

# project name
PROJECT( SIO )

# project version
SET( SIO_VERSION_MAJOR ${LCIO_VERSION_MAJOR} )
SET( SIO_VERSION_MINOR ${LCIO_VERSION_MINOR} )
SET( SIO_VERSION_PATCH ${LCIO_VERSION_PATCH} )
SET( SIO_SOVERSION "${SIO_VERSION_MAJOR}.${SIO_VERSION_MINOR}" )
SET( SIO_VERSION "${SIO_SOVERSION}.${SIO_VERSION_PATCH}" )

# options
OPTION( SIO_EXAMPLES "Switch ON to build SIO examples" ON )
SET( SIO_LOGLVL "0" CACHE STRING "The SIO verbosity level" )

IF( NOT SIO_LOGLVL MATCHES "^[0-9]+$" )
    MESSAGE( SEND_ERROR "SIO log level variable not a number (${SIO_LOGLVL})" )
ENDIF()

ADD_DEFINITIONS( "-DSIO_LOGLVL=${SIO_LOGLVL}" )

# ZLIB package
FIND_PACKAGE( ZLIB REQUIRED )
#ZLIB_INCLUDE_DIR - where to find zlib.h, etc.
#ZLIB_LIBRARIES   - List of libraries when using zlib.
#ZLIB_FOUND       - True if zlib found.

INCLUDE( TestBigEndian )
TEST_BIG_ENDIAN( BIG_ENDIAN )

IF( NOT BIG_ENDIAN )
    ADD_DEFINITIONS( "-D__LITTLE_ENDIAN__" )
ENDIF()

INCLUDE_DIRECTORIES(
    ${SIO_SOURCE_DIR}/include
    ${ZLIB_INCLUDE_DIR}
)

FILE( GLOB SIO_SRCS src/*.cc )

IF( SIO_PRINT_DEBUG )
  ADD_DEFINITIONS( "-DSIO_PRINT_DEBUG" )
ENDIF()

ADD_SHARED_LIBRARY( sio ${SIO_SRCS} )
TARGET_LINK_LIBRARIES( sio ${ZLIB_LIBRARIES} )
INSTALL_SHARED_LIBRARY( sio DESTINATION lib )

# install SIO headers
INSTALL_DIRECTORY( include/ DESTINATION include )

ADD_EXECUTABLE( sio-dump main/sio-dump.cc )
TARGET_LINK_LIBRARIES( sio-dump sio )
INSTALL( TARGETS sio-dump RUNTIME DESTINATION bin )

ADD_EXECUTABLE( sio-dump-detailed main/sio-dump-detailed.cc )
TARGET_LINK_LIBRARIES( sio-dump-detailed sio )
INSTALL( TARGETS sio-dump-detailed RUNTIME DESTINATION bin )

function ( add_test_reg test_name )
  cmake_parse_arguments(ARG "BUILD_EXEC" "" "COMMAND;DEPENDS;EXEC_ARGS;REGEX_PASS;REGEX_PASSED;REGEX_FAIL;REGEX_FAILED;REQUIRES" ${ARGN} )
  set ( missing )
  set ( use_test 1 )

  if ( ${ARG_BUILD_EXEC} )
    add_executable ( ${test_name} tests/${test_name}.cc )
    target_link_libraries( ${test_name} sio )
    target_include_directories( ${test_name} PUBLIC ${SIO_SOURCE_DIR}/tests )
    set_target_properties( ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )
  endif()

  set ( cmd ${ARG_COMMAND} )
  if ( "${cmd}" STREQUAL "" )
    if( ${ARG_BUILD_EXEC} )
      set ( cmd ${CMAKE_BINARY_DIR}/bin/${test_name} )
    else()
      message( SEND_ERROR "No command no executable provided" )
    endif()
  endif()

  set ( passed ${ARG_REGEX_PASS} ${ARG_REGEX_PASSED} )
  if ( "${passed}" STREQUAL "NONE" )
    unset ( passed )
  elseif ( "${passed}" STREQUAL "" )
    set ( passed "TEST_PASSED" )
  endif()

  set ( failed ${ARG_REGEX_FAIL} ${ARG_REGEX_FAILED} )
  if ( "${failed}" STREQUAL "NONE" )
    unset ( failed )
  endif()

  set ( args ${ARG_EXEC_ARGS} )
  add_test(NAME t_${test_name} COMMAND ${cmd} ${args} )
  if ( NOT "${passed}" STREQUAL "" )
    set_tests_properties( t_${test_name} PROPERTIES PASS_REGULAR_EXPRESSION "${passed}" )
  endif()
  if ( NOT "${failed}" STREQUAL "" )
    set_tests_properties( t_${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION "${failed}" )
  endif()
  # Set test dependencies if present
  foreach ( _dep ${ARG_DEPENDS} )
    set_tests_properties( t_${test_name} PROPERTIES DEPENDS t_${_dep} )
  endforeach()
endfunction()


add_test_reg (
  test_exception
  BUILD_EXEC 
  REGEX_FAIL "TEST_FAILED" 
)

IF( SIO_EXAMPLES )
  ADD_SUBDIRECTORY( examples )
ENDIF()
