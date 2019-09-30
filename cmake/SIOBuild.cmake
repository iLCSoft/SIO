
# Policy for "Support new ``if()`` IN_LIST operator"
CMAKE_POLICY( SET CMP0057 NEW )

MACRO( SIO_ADD_SHARED_LIBRARY _name )
  ADD_LIBRARY( ${_name} SHARED ${ARGN} )
  # change lib_target properties
  SET_TARGET_PROPERTIES( ${_name} PROPERTIES
    # create *nix style library versions + symbolic links
    VERSION ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_SOVERSION}
  )
ENDMACRO()

# wrapper macro for installing a shared library with
# correct permissions
MACRO( SIO_INSTALL_SHARED_LIBRARY )
  # install library
  INSTALL( TARGETS ${ARGN}
    PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
  )
ENDMACRO()

# wrapper macro to install a directory
# excluding any backup, temporary files
# and .svn / CVS directories
MACRO( SIO_INSTALL_DIRECTORY )
    INSTALL( DIRECTORY ${ARGN}
        PATTERN "*~" EXCLUDE
        PATTERN "*#*" EXCLUDE
        PATTERN ".#*" EXCLUDE
        PATTERN "*CVS" EXCLUDE
        PATTERN "*.svn" EXCLUDE
    )
ENDMACRO()

FUNCTION( SIO_GENERATE_DOXYFILE )
  CMAKE_PARSE_ARGUMENTS( ARG "" "OUTPUT_DIR" "INPUT" ${ARGN} )
  # the input directory where the Doxyfile.in is
  SET( DOXYFILE_INPUT_DIR "${PROJECT_SOURCE_DIR}/cmake" )
  # the output location of the Doxyfile 
  SET( DOXYFILE_OUTPUT_DIR "${PROJECT_BINARY_DIR}/Doxygen" )
  IF( ARG_OUTPUT_DIR )
    SET( DOXYFILE_OUTPUT_DIR ${ARG_OUTPUT_DIR} )
  ENDIF()
  # the doxygen input source location
  SET( DOX_INPUT ${ARG_INPUT} )
  # the project version
  SET( DOX_PROJECT_NUMBER "${${PROJECT_NAME}_VERSION}" )
  # output path of doxygen output (html, latex, ...)
  SET( DOX_OUTPUT_DIRECTORY "${DOXYFILE_OUTPUT_DIR}" )
  # configure file to output location
  FILE( MAKE_DIRECTORY ${DOXYFILE_OUTPUT_DIR} )
  CONFIGURE_FILE( "${DOXYFILE_INPUT_DIR}/Doxyfile.in" "${DOXYFILE_OUTPUT_DIR}/Doxyfile" @ONLY )
ENDFUNCTION()


# helper macro to display standard cmake variables and force write to cache
# otherwise outdated values may appear in ccmake gui
MACRO( SIO_DISPLAY_STD_VARIABLES )
  MESSAGE( STATUS )
  MESSAGE( STATUS "-------------------------------------------------------------------------------" )
  MESSAGE( STATUS "Change values with: cmake -D<Variable>=<Value>" )
  IF( DEFINED CMAKE_INSTALL_PREFIX )
    MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
  ENDIF()
  IF( DEFINED CMAKE_BUILD_TYPE )
    MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
  ENDIF()
  IF( DEFINED BUILD_SHARED_LIBS )
    MESSAGE( STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}" )
  ENDIF()
  IF( DEFINED BUILD_TESTING )
    MESSAGE( STATUS "BUILD_TESTING = ${BUILD_TESTING}" )
  ENDIF()
  IF( DEFINED INSTALL_DOC )
    MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
  ENDIF()
  IF( DEFINED CMAKE_PREFIX_PATH )
    LIST( REMOVE_DUPLICATES CMAKE_PREFIX_PATH )
    MESSAGE( STATUS "CMAKE_PREFIX_PATH =" )
    FOREACH( _path ${CMAKE_PREFIX_PATH} )
        MESSAGE( STATUS "   ${_path};" )
    ENDFOREACH()
  ENDIF()
  IF( DEFINED CMAKE_MODULE_PATH )
    LIST( REMOVE_DUPLICATES CMAKE_MODULE_PATH )
    MESSAGE( STATUS "CMAKE_MODULE_PATH =" )
    FOREACH( _path ${CMAKE_MODULE_PATH} )
        MESSAGE( STATUS "   ${_path};" )
    ENDFOREACH()
    SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE PATH "CMAKE_MODULE_PATH" FORCE )
  ENDIF()
  MESSAGE( STATUS "-------------------------------------------------------------------------------" )
  MESSAGE( STATUS )
ENDMACRO()



# helper macro for generating project configuration file
MACRO( SIO_GENERATE_PACKAGE_CONFIGURATION_FILES )
  FOREACH( arg ${ARGN} )
    IF( ${arg} MATCHES "Config.cmake" )
      IF( EXISTS "${PROJECT_SOURCE_DIR}/cmake/${arg}.in" )
        CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${arg}.in"
                        "${PROJECT_BINARY_DIR}/${arg}" @ONLY
        )
        INSTALL( FILES "${PROJECT_BINARY_DIR}/${arg}" DESTINATION . )
      ENDIF()
    ENDIF()
    IF( ${arg} MATCHES "ConfigVersion.cmake" )
      # version configuration file
      IF( EXISTS "${PROJECT_SOURCE_DIR}/cmake/${arg}.in" )
        CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${arg}.in"
                        "${PROJECT_BINARY_DIR}/${arg}" @ONLY
        )
        INSTALL( FILES "${PROJECT_BINARY_DIR}/${arg}" DESTINATION . )
      ENDIF()
    ENDIF()
  ENDFOREACH()
ENDMACRO()


function ( SIO_ADD_TEST_REG test_name )
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

# set default install prefix to project root directory
# instead of the cmake default /usr/local
IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
 	SET(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" CACHE PATH "Where to install ${PROJECT_NAME}" FORCE)
ENDIF()

# set default cmake build type to RelWithDebInfo
# possible options are: None Debug Release RelWithDebInfo MinSizeRel
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# write this variable to cache
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )

# enable ctest
ENABLE_TESTING()
INCLUDE(CTest)
MARK_AS_ADVANCED( DART_TESTING_TIMEOUT )

# library *nix style versioning
SET( ${PROJECT_NAME}_SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
SET( ${PROJECT_NAME}_VERSION   "${${PROJECT_NAME}_SOVERSION}.${${PROJECT_NAME}_VERSION_PATCH}" )

# output directories
SET( EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin" )
SET( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH )
MARK_AS_ADVANCED( LIBRARY_OUTPUT_PATH )

# add library install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# add install path to the rpath list (apple)
IF( APPLE )
  SET( CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib" )
  MARK_AS_ADVANCED( CMAKE_INSTALL_NAME_DIR )
ENDIF()

# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )

## Set the default compiler flags for all projects picking up default ilcutil settings
## This runs checks if compilers support the flag and sets them, if they do
## this will create a humongous amount of warnings when compiling :)
INCLUDE(CheckCXXCompilerFlag)

SET( COMPILER_FLAGS
  -Wformat-security
  -Wreturn-type
  -Wlogical-op
  -Wredundant-decls
  -Wno-deprecated-declarations
  -Wall
  -Wunused-value
  -Wextra
  -Wshadow
  -Weffc++
  -pedantic
  -Wno-long-long
  -Wuninitialized
  -Wno-non-virtual-dtor
  -Wheader-hygiene
)

IF( SIO_PROFILING AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  LIST( APPEND COMPILER_FLAGS -pg )
ENDIF()

IF( SIO_WITH_OFAST )
  SET( CMAKE_CXX_FLAGS_RELEASE "-Ofast" )
ELSE()
  SET( CMAKE_CXX_FLAGS_RELEASE "-O3" )
ENDIF()

IF( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
 LIST( APPEND COMPILER_FLAGS -Wl,-no-undefined )
ENDIF()

# Dealing with CXX standard
SET( SIO_POSSIBLE_CXX_STANDARDS 11 14 17 20 )
LIST( GET SIO_POSSIBLE_CXX_STANDARDS 0 SIO_MINIMUM_CXX_REQUIRED )

IF( CMAKE_CXX_STANDARD )
  MESSAGE( STATUS "Checking CXX standard set by user ..." )
  IF( NOT ${CMAKE_CXX_STANDARD} IN_LIST SIO_POSSIBLE_CXX_STANDARDS )
    MESSAGE( FATAL_ERROR "Invalid cxx standard (set=${CMAKE_CXX_STANDARD}, minimum=${SIO_MINIMUM_CXX_REQUIRED})" )
  ENDIF()
ELSE()
  MESSAGE( STATUS "Setting CXX standard to minimum required ..." )
  SET( CMAKE_CXX_STANDARD ${SIO_MINIMUM_CXX_REQUIRED} )
ENDIF()

MESSAGE( STATUS "Using CXX standard ${CMAKE_CXX_STANDARD}" )

MESSAGE( STATUS "FLAGS ${COMPILER_FLAGS}" )
FOREACH( FLAG ${COMPILER_FLAGS} )
  ## meed to replace the minus or plus signs from the variables, because it is passed
  ## as a macro to g++ which causes a warning about no whitespace after macro
  ## definition
  STRING(REPLACE "-" "_" FLAG_WORD ${FLAG} )
  STRING(REPLACE "+" "P" FLAG_WORD ${FLAG_WORD} )

  CHECK_CXX_COMPILER_FLAG( "${FLAG}" CXX_FLAG_WORKS_${FLAG_WORD} )
  IF( ${CXX_FLAG_WORKS_${FLAG_WORD}} )
    MESSAGE ( STATUS "Adding ${FLAG} to CXX_FLAGS" )
    ### We prepend the flag, so they are overwritten by whatever the user sets in his own configuration
    SET ( CMAKE_CXX_FLAGS "${FLAG} ${CMAKE_CXX_FLAGS}")
  ELSE()
    MESSAGE ( STATUS "NOT Adding ${FLAG} to CXX_FLAGS" )
  ENDIF()
ENDFOREACH()

IF( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9 )
  SET( CMAKE_CXX_FLAGS "-fdiagnostics-color=auto ${CMAKE_CXX_FLAGS}" )
ENDIF()

INSTALL( FILES cmake/MacroCheckPackageLibs.cmake cmake/MacroCheckPackageVersion.cmake DESTINATION lib/cmake )
