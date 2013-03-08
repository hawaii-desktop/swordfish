add_definitions(-DQT_NO_KEYWORDS -Demit=)
include_directories( 
	${GLIB2_INCLUDE_DIRS} 
	${GIO_INCLUDE_DIRS} 
	${GOBJECT_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/3rdparty/kommodity/include
	)

set(kommoditygio_LIB_SRCS
	kgio-wrappedobject.cpp 
	kgio-sharedwrappedobject.cpp 
	kgio-file.cpp
	kgio-fileasyncoperation.cpp  
	kgio-error.cpp
	kgio-seekable.cpp  
	kgio-cancellable.cpp
	kgio-inputstream.cpp
	kgio-outputstream.cpp	
	kgio-fileinputstream.cpp
	kgio-fileoutputstream.cpp
	kgio-fileinfo.cpp
	kgio-mountoperation.cpp
	kgio-mount.cpp
	kgio-drive.cpp
	kgio-volume.cpp
	kgio-volumemonitor.cpp
	kgio-filemonitor.cpp
	kgio-fileenumerator.cpp
	kgio-wrappedobjectutils.cpp
	kgio-icon.cpp
	kgio-themedicon.cpp
	kgio-loadableicon.cpp
	kgio-appinfo.cpp
	kgio-applaunchcontext.cpp
	kgio-contenttype.cpp
	kgio-fileicon.cpp
	kgio-filedevice.cpp
	kgio-fileattributematcher.cpp
	)


add_library(kommoditygio STATIC ${kommoditygio_LIB_SRCS})
qt5_use_modules(kommoditygio Core)
target_link_libraries(kommoditygio ${GLIB2_LIBRARIES} ${GIO_LIBRARIES} ${GOBJECT_LIBRARIES})

set(kommodity_gio_public_HDRS
kgio-appinfo.h
kgio-applaunchcontext.h
kgio-cancellable.h
kgio-contenttype.h
kgio-drive.h
kgio-error.h
kgio-fileattribute.h
kgio-filedevice.h
kgio-fileenumerator.h
kgio-file.h
kgio-fileicon.h
kgio-fileinfo.h
kgio-fileinputstream.h
kgio-filemonitor.h
kgio-fileoutputstream.h
kgio-icon.h
kgio-inputstream.h
kgio-ioerror.h
kgio-loadableicon.h
kgio-mount.h
kgio-mountoperation.h
kgio-outputstream.h
kgio-seekable.h
kgio-themedicon.h
kgio-volume.h
kgio-volumemonitor.h
kgio-wrappedobject.h
kgio-sharedwrappedobject.h
kgio-fileattributematcher.h
)
