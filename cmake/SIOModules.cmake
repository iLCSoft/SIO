#---------------------------------------------------------------------------------------------------
# CMake support for C++20 modules in SIO
#
# This file provides macros and options for generating and using C++20 module interfaces
# for the SIO library.
#---------------------------------------------------------------------------------------------------

# Option to enable C++20 module generation
option(SIO_ENABLE_CXX_MODULES "Generate C++20 module interface files (.ixx) for SIO" OFF)

# Check if modules are actually supported
if(SIO_ENABLE_CXX_MODULES)
  # Check CMake version
  if(CMAKE_VERSION VERSION_LESS 3.29)
    message(WARNING "C++20 modules require CMake 3.29 or later (found ${CMAKE_VERSION}). Disabling module support.")
    set(SIO_ENABLE_CXX_MODULES OFF CACHE BOOL "C++20 modules disabled due to CMake version" FORCE)
  endif()

  # Check generator
  if(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
    message(WARNING "C++20 modules are not supported with the Unix Makefiles generator. Please use Ninja. Disabling module support.")
    set(SIO_ENABLE_CXX_MODULES OFF CACHE BOOL "C++20 modules disabled due to generator" FORCE)
  endif()

  # Check compiler support
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0)
      message(WARNING "C++20 modules require GCC 14 or later (found ${CMAKE_CXX_COMPILER_VERSION}). Disabling module support.")
      set(SIO_ENABLE_CXX_MODULES OFF CACHE BOOL "C++20 modules disabled due to compiler version" FORCE)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18.0)
      message(WARNING "C++20 modules require Clang 18 or later (found ${CMAKE_CXX_COMPILER_VERSION}). Disabling module support.")
      set(SIO_ENABLE_CXX_MODULES OFF CACHE BOOL "C++20 modules disabled due to compiler version" FORCE)
    endif()
  else()
    message(WARNING "C++20 modules have only been tested with GCC 14+ and Clang 18+. Found ${CMAKE_CXX_COMPILER_ID}. Proceed at your own risk.")
  endif()
endif()

if(SIO_ENABLE_CXX_MODULES)
  message(STATUS "C++20 module support is ENABLED for SIO")
  message(STATUS "  - CMake version: ${CMAKE_VERSION}")
  message(STATUS "  - Generator: ${CMAKE_GENERATOR}")
  message(STATUS "  - Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
else()
  message(STATUS "C++20 module support is DISABLED for SIO")
endif()

#---------------------------------------------------------------------------------------------------
# SIO_ADD_MODULE_INTERFACE( target module_file )
#
# Add a C++20 module interface to the SIO target
#
# Arguments:
#   target      - The target to which the module should be added
#   module_file - The .ixx file implementing the module interface
#---------------------------------------------------------------------------------------------------
function(SIO_ADD_MODULE_INTERFACE target module_file)
  if(NOT SIO_ENABLE_CXX_MODULES)
    return()
  endif()

  message(STATUS "Adding C++20 module 'sio' to target '${target}'")

  # Add the module file to the target as a CXX_MODULES file set
  # Use PUBLIC so the module gets installed and can be used by downstream projects
  target_sources(${target}
    PUBLIC
      FILE_SET CXX_MODULES
      BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
      FILES ${module_file}
  )
endfunction()
