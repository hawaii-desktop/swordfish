
include_directories(
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

QT5_USE_MODULES(kgio-readfile-sample Widgets)

TARGET_LINK_LIBRARIES(kgio-readfile-sample
				      kommoditygio
)

#######

SET(kgio-createfile-sample_SRCS
    kgio-createfile-sample.cpp
)

ADD_EXECUTABLE(kgio-createfile-sample
               ${kgio-createfile-sample_SRCS}
)

QT5_USE_MODULES(kgio-createfile-sample Widgets)

TARGET_LINK_LIBRARIES(kgio-createfile-sample
				      kommoditygio
)

#######

SET(kgio-readfileasync-sample_SRCS
    kgio-readfileasync-sample.cpp
)

ADD_EXECUTABLE(kgio-readfileasync-sample
               ${kgio-readfileasync-sample_SRCS}
)

QT5_USE_MODULES(kgio-readfileasync-sample Widgets)

TARGET_LINK_LIBRARIES(kgio-readfileasync-sample
				      kommoditygio
)

#######

SET(kgio-enumeratedir-sample_SRCS
    kgio-enumeratedir-sample.cpp
)

ADD_EXECUTABLE(kgio-enumeratedir-sample
               ${kgio-enumeratedir-sample_SRCS}
)

QT5_USE_MODULES(kgio-enumeratedir-sample Widgets)

TARGET_LINK_LIBRARIES(kgio-enumeratedir-sample
				      kommoditygio
)

#######

SET(kgio-fileinfo-sample_SRCS
    kgio-fileinfo-sample.cpp
)

ADD_EXECUTABLE(kgio-fileinfo-sample
               ${kgio-fileinfo-sample_SRCS}
)

QT5_USE_MODULES(kgio-fileinfo-sample Widgets)

TARGET_LINK_LIBRARIES(kgio-fileinfo-sample
				      kommoditygio
)

#######

SET(kgio-volumemonitor-sample_SRCS
    kgio-volumemonitor-sample.cpp
)

ADD_EXECUTABLE(kgio-volumemonitor-sample
               ${kgio-volumemonitor-sample_SRCS}
)

QT5_USE_MODULES(kgio-volumemonitor-sample Widgets)

TARGET_LINK_LIBRARIES(kgio-volumemonitor-sample
				      kommoditygio
)

#######

SET(kgio-mountoperation-sample_SRCS
    kgio-mountoperation-sample.cpp
)

ADD_EXECUTABLE(kgio-mountoperation-sample
               ${kgio-mountoperation-sample_SRCS}
)

QT5_USE_MODULES(kgio-mountoperation-sample Widgets)

TARGET_LINK_LIBRARIES(kgio-mountoperation-sample
				      kommoditygio
)

#######

SET(kgio-iodevice-sample_SRCS
    kgio-iodevice-sample.cpp
)

ADD_EXECUTABLE(kgio-iodevice-sample
               ${kgio-iodevice-sample_SRCS}
)

QT5_USE_MODULES(kgio-iodevice-sample Widgets Xml)

TARGET_LINK_LIBRARIES(kgio-iodevice-sample
				      kommoditygio
)

#######

add_subdirectory( go )

