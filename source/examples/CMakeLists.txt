

INCLUDE_DIRECTORIES( BEFORE common/include )

# simple example
ADD_EXECUTABLE( simple_write simple/simple_write.cc )
TARGET_LINK_LIBRARIES( simple_write sio )
INSTALL( TARGETS simple_write RUNTIME DESTINATION bin/examples )

ADD_EXECUTABLE( simple_read simple/simple_read.cc )
TARGET_LINK_LIBRARIES( simple_read sio )
INSTALL( TARGETS simple_read RUNTIME DESTINATION bin/examples )

# zlib example
ADD_EXECUTABLE( zlib_write zlib/zlib_write.cc )
TARGET_LINK_LIBRARIES( zlib_write sio )
INSTALL( TARGETS zlib_write RUNTIME DESTINATION bin/examples )

ADD_EXECUTABLE( zlib_read zlib/zlib_read.cc )
TARGET_LINK_LIBRARIES( zlib_read sio )
INSTALL( TARGETS zlib_read RUNTIME DESTINATION bin/examples )


# relocation example
ADD_EXECUTABLE( relocation_write relocation/relocation_write.cc )
TARGET_LINK_LIBRARIES( relocation_write sio )
INSTALL( TARGETS relocation_write RUNTIME DESTINATION bin/examples )

ADD_EXECUTABLE( relocation_read relocation/relocation_read.cc )
TARGET_LINK_LIBRARIES( relocation_read sio )
INSTALL( TARGETS relocation_read RUNTIME DESTINATION bin/examples )


