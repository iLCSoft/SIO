###############################################
# cmake file for building SIO documentation
# @author Jan Engels, DESY
# @author Remi Ete, DESY
###############################################

# required packages to build documentation
FIND_PACKAGE( Doxygen REQUIRED )
IF( NOT DOXYGEN_FOUND )
  MESSAGE( SEND_ERROR "Failed to find doxygen required to build SIO C++ API documentation" )
  MESSAGE( "Please install doxygen or set INSTALL_DOC to OFF" )
  MESSAGE( "" )
ENDIF()

# get documentation sources
SET( SIO_HEADER_DIR ${PROJECT_SOURCE_DIR}/source/include/sio )
SET( SIO_SRC_DIR ${PROJECT_SOURCE_DIR}/source/src )
FILE( GLOB_RECURSE SIO_HEADER_LIST ${SIO_HEADER_DIR}/*.h )
FILE( GLOB_RECURSE SIO_SRC_LIST ${SIO_SRC_DIR}/*.cc )
SET( SIO_ALL_DOC_SOURCES ${SIO_HEADER_LIST} ${SIO_SRC_LIST} )

# generate our Doxyfile
SET( DOXYFILE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/Doxygen )
SET( DOXYFILE_INPUT "${SIO_HEADER_DIR} ${SIO_SRC_DIR}" )
SIO_GENERATE_DOXYFILE( OUTPUT_DIR ${DOXYFILE_OUTPUT_DIR} INPUT ${DOXYFILE_INPUT} )

# tell make clean to erase DOC_BIN_DIR
SET_DIRECTORY_PROPERTIES( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${DOXYFILE_OUTPUT_DIR} )

# add custom target to build all documentation
ADD_CUSTOM_TARGET( doc )

# documentation install directory
SET( DOC_INSTALL_PREFIX doc/Doxygen )

ADD_CUSTOM_COMMAND(
  OUTPUT  "${DOXYFILE_OUTPUT_DIR}/html/index.html"
  COMMAND "${DOXYGEN_EXECUTABLE}"
  WORKING_DIRECTORY ${DOXYFILE_OUTPUT_DIR}
  COMMENT "Building C++ API Documentation..."
  DEPENDS ${DOXYFILE_OUTPUT_DIR}/Doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt ${SIO_ALL_DOC_SOURCES}
)
ADD_CUSTOM_TARGET( doc_cpp DEPENDS "${DOXYFILE_OUTPUT_DIR}/html/index.html" )
ADD_DEPENDENCIES( doc doc_cpp )
SIO_INSTALL_DIRECTORY( "${DOXYFILE_OUTPUT_DIR}/html" DESTINATION ${DOC_INSTALL_PREFIX} )
