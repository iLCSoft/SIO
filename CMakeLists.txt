########################################################
# cmake file for building SIO
# @author Frank Gaede, DESY
# @author Jan Engels, DESY
# @author Remi Ete, DESY
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 FATAL_ERROR )
########################################################

# project name
PROJECT( SIO )

# project version
SET( SIO_VERSION_MAJOR 2 )
SET( SIO_VERSION_MINOR 12 )
SET( SIO_VERSION_PATCH 1 )

### DEPENDENCIES ############################################################

LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

# ZLIB package
FIND_PACKAGE( ZLIB REQUIRED )
INCLUDE_DIRECTORIES( SYSTEM ${ZLIB_INCLUDE_DIR} )

# options
OPTION( SIO_EXAMPLES        "Switch ON to build SIO examples" ON )
OPTION( INSTALL_DOC         "Set to OFF to skip build/install Documentation" OFF )
OPTION( SIO_WITH_SIMD       "Set to OFF to disable SIMD cxx flags" ON )
OPTION( SIO_WITH_OFAST      "Set to OFF to disable -Ofast cxx flags" OFF )
SET(    SIO_LOGLVL          "0" CACHE STRING "The SIO verbosity level" )

IF( NOT SIO_LOGLVL MATCHES "^[0-9]+$" )
  MESSAGE( SEND_ERROR "SIO log level variable not a number (${SIO_LOGLVL})" )
ENDIF()

# load SIO build settings
INCLUDE( SIOBuild )

# sio library
ADD_SUBDIRECTORY( source )

### DOCUMENTATION ###########################################################
IF( INSTALL_DOC )
    ADD_SUBDIRECTORY( ./doc )
    INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} doc)" )
ENDIF()

# display some variables and write them to cache
SIO_DISPLAY_STD_VARIABLES()

# generate and install following configuration files
SIO_GENERATE_PACKAGE_CONFIGURATION_FILES( SIOConfig.cmake SIOConfigVersion.cmake )
