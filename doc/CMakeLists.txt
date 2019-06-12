###############################################
# cmake file for building SIO documentation
# @author Jan Engels, DESY
# @author Remi Ete, DESY
###############################################

# build documentation out-of-source
SET( DOC_SRC_DIR "${PROJECT_SOURCE_DIR}/doc" )
SET( DOC_BIN_DIR "${PROJECT_BINARY_DIR}/docbuild" )

# tell make clean to erase DOC_BIN_DIR
SET_DIRECTORY_PROPERTIES( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${DOC_BIN_DIR}" )

# add custom target to build all documentation
ADD_CUSTOM_TARGET( doc )

# required packages to build documentation
FIND_PACKAGE( Doxygen REQUIRED )

# out-of-source vs in-source installations
IF( "${CMAKE_INSTALL_PREFIX}" STREQUAL "${PROJECT_SOURCE_DIR}" )
  SET( DOC_INSTALL_PREFIX doc )
ELSE()
  SET( DOC_INSTALL_PREFIX doc/sio )
ENDIF()

# ---------- c++ documentation -----------------------------------------------
IF( DOXYGEN_FOUND )
  ADD_CUSTOM_COMMAND(
    OUTPUT  "${DOC_BIN_DIR}/html/index.html"
    COMMAND DOX_PROJECT_NUMBER="${${PROJECT_NAME}_VERSION}"
            DOX_OUTPUT_DIRECTORY="${DOC_BIN_DIR}"
            DOX_INPUT="${PROJECT_SOURCE_DIR}/sio/include/sio"
            "${DOXYGEN_EXECUTABLE}"
    WORKING_DIRECTORY ${DOC_SRC_DIR}
    COMMENT "Building C++ API Documentation..."
    DEPENDS Doxyfile CMakeLists.txt ${PROJECT_SOURCE_DIR}/sio/include/*
  )
  ADD_CUSTOM_TARGET( doc_cpp DEPENDS "${DOC_BIN_DIR}/html/index.html" )
  ADD_DEPENDENCIES( doc doc_cpp )
  SIO_INSTALL_DIRECTORY( "${DOC_BIN_DIR}/html" DESTINATION ${DOC_INSTALL_PREFIX}/doxygen_api )
ELSE()
  MESSAGE( SEND_ERROR "Failed to find doxygen required to build SIO C++ API documentation" )
  MESSAGE( "Please install doxygen or set INSTALL_DOC to OFF" )
  MESSAGE( "" )
ENDIF()

