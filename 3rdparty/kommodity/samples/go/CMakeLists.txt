
include_directories( 
	${QT_INCLUDES} 
	${GLIB2_INCLUDE_DIR} 
	${GIO_INCLUDE_DIR} 
	${GOBJECT_INCLUDE_DIR}
	${kommodity_BINARY_DIR}/library/gio
	${kommodity_SOURCE_DIR}/library/gio
	${kommodity_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}
	)

SET(go-wrapper-sample_SRCS
    go-wrapper-sample.cpp
)

ADD_EXECUTABLE(go-wrapper-sample
               ${go-wrapper-sample_SRCS}
)

TARGET_LINK_LIBRARIES(go-wrapper-sample
				      ${QT_QTGUI_LIBRARY} 
				      kommoditygio
)



