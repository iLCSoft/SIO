

INCLUDE_DIRECTORIES( BEFORE common/include )

# simple example
ADD_EXECUTABLE( simple_write simple/simple_write.cc )
TARGET_LINK_LIBRARIES( simple_write sio )
INSTALL( TARGETS simple_write RUNTIME DESTINATION bin/examples )

# simple example
ADD_EXECUTABLE( simple_read simple/simple_read.cc )
TARGET_LINK_LIBRARIES( simple_read sio )
INSTALL( TARGETS simple_read RUNTIME DESTINATION bin/examples )


