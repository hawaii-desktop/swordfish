
include_directories(
	${QT_INCLUDES} 
	${kommodity_SOURCE_DIR}/include
	${kommodity_BINARY_DIR}/library/gio
	${kommodity_SOURCE_DIR}/library/gio
	${CMAKE_CURRENT_BINARY_DIR}
	)

#######

SET(kgio-readfile-sample_SRCS
    kgio-readfile-sample.cpp
)

ADD_EXECUTABLE(kgio-readfile-sample
               ${kgio-readfile-sample_SRCS}
)

TARGET_LINK_LIBRARIES(kgio-readfile-sample
				      ${QT_QTGUI_LIBRARY}
				      kommoditygio
)

#######

SET(kgio-createfile-sample_SRCS
    kgio-createfile-sample.cpp
)

ADD_EXECUTABLE(kgio-createfile-sample
               ${kgio-createfile-sample_SRCS}
)

TARGET_LINK_LIBRARIES(kgio-createfile-sample
				      ${QT_QTGUI_LIBRARY}
				      kommoditygio
)

#######

SET(kgio-readfileasync-sample_SRCS
    kgio-readfileasync-sample.cpp
)

QT4_AUTOMOC(${kgio-readfileasync-sample_SRCS})

ADD_EXECUTABLE(kgio-readfileasync-sample
               ${kgio-readfileasync-sample_SRCS}
)

TARGET_LINK_LIBRARIES(kgio-readfileasync-sample
				      ${QT_QTGUI_LIBRARY}
				      kommoditygio
)

#######

SET(kgio-enumeratedir-sample_SRCS
    kgio-enumeratedir-sample.cpp
)

ADD_EXECUTABLE(kgio-enumeratedir-sample
               ${kgio-enumeratedir-sample_SRCS}
)

TARGET_LINK_LIBRARIES(kgio-enumeratedir-sample
				      ${QT_QTGUI_LIBRARY}
				      kommoditygio
)

#######

SET(kgio-fileinfo-sample_SRCS
    kgio-fileinfo-sample.cpp
)

ADD_EXECUTABLE(kgio-fileinfo-sample
               ${kgio-fileinfo-sample_SRCS}
)

TARGET_LINK_LIBRARIES(kgio-fileinfo-sample
				      ${QT_QTGUI_LIBRARY}
				      kommoditygio
)

#######

SET(kgio-volumemonitor-sample_SRCS
    kgio-volumemonitor-sample.cpp
)

QT4_AUTOMOC(${kgio-volumemonitor-sample_SRCS})

ADD_EXECUTABLE(kgio-volumemonitor-sample
               ${kgio-volumemonitor-sample_SRCS}
)

TARGET_LINK_LIBRARIES(kgio-volumemonitor-sample
				      ${QT_QTGUI_LIBRARY}
				      kommoditygio
)

#######

SET(kgio-mountoperation-sample_SRCS
    kgio-mountoperation-sample.cpp
)

QT4_AUTOMOC(${kgio-mountoperation-sample_SRCS})

ADD_EXECUTABLE(kgio-mountoperation-sample
               ${kgio-mountoperation-sample_SRCS}
)

TARGET_LINK_LIBRARIES(kgio-mountoperation-sample
				      ${QT_QTGUI_LIBRARY}
				      kommoditygio
)

#######

SET(kgio-iodevice-sample_SRCS
    kgio-iodevice-sample.cpp
)

ADD_EXECUTABLE(kgio-iodevice-sample
               ${kgio-iodevice-sample_SRCS}
)

TARGET_LINK_LIBRARIES(kgio-iodevice-sample
				      ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY}
				      kommoditygio
)

#######

add_subdirectory( go )

