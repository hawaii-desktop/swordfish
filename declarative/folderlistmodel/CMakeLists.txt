include_directories(
    disk
    trash
    net
    smb
    smb/qsambaclient/src
)

add_definitions(
    -DDEBUG_MESSAGES=0
    -DDEBUG_EXT_FS_WATCHER=0
    -DDEBUG_REMOVE=0
    -DDO_NOT_USE_TAG_LIB=1
    -DREGRESSION_TEST_FOLDERLISTMODEL=0
)

set(SOURCES
    clipboard.cpp
    clipboard.h
    diritemabstractlistmodel.h
    diriteminfo.cpp
    diriteminfo.h
    dirmodel.cpp
    dirmodel.h
    dirselection.cpp
    dirselection.h
    externalfswatcher.cpp
    externalfswatcher.h
    filecompare.cpp
    filecompare.h
    filesystemaction.cpp
    filesystemaction.h
    fmutil.cpp
    fmutil.h
    imageprovider.cpp
    imageprovider.h
    iorequest.cpp
    iorequest.h
    iorequestworker.cpp
    iorequestworker.h
    ioworkerthread.cpp
    ioworkerthread.h
    networklistworker.cpp
    networklistworker.h
    plugin.cpp
    plugin.h
    location.cpp
    location.h
    locationsfactory.cpp
    locationsfactory.h
    locationurl.cpp
    locationurl.h
    networklocation.cpp
    networklocation.h
    locationitemdir.cpp
    locationitemdir.h
    locationitemdiriterator.cpp
    locationitemdiriterator.h
    locationitemfile.cpp
    locationitemfile.h
    cleanurl.cpp
    cleanurl.h
    urliteminfo.cpp
    urliteminfo.h
    disk/disklocation.cpp
    disk/disklocation.h
    disk/disklocationitemdiriterator.cpp
    disk/disklocationitemdiriterator.h
    disk/disklocationitemfile.cpp
    disk/disklocationitemfile.h
    disk/disklocationitemdir.cpp
    disk/disklocationitemdir.h
    trash/qtrashdir.cpp
    trash/qtrashdir.h
    trash/qtrashutilinfo.cpp
    trash/qtrashutilinfo.h
    trash/trashiteminfo.cpp
    trash/trashiteminfo.h
    trash/trashlocation.cpp
    trash/trashlocation.h
    net/netauthenticationdata.cpp
    net/netauthenticationdata.h
    net/netutil.cpp
    net/netutil.h
)

set(SAMBA_SOURCES
    smb/qsambaclient/src/smbutil.cpp
    smb/qsambaclient/src/smbutil.h
    smb/qsambaclient/src/smbiteminfo.cpp
    smb/qsambaclient/src/smbiteminfo.h
    smb/qsambaclient/src/smblocationdiriterator.cpp
    smb/qsambaclient/src/smblocationdiriterator.h
    smb/qsambaclient/src/smblocationitemfile.cpp
    smb/qsambaclient/src/smblocationitemfile.h
    smb/qsambaclient/src/smblocationitemdir.cpp
    smb/qsambaclient/src/smblocationitemdir.h
    smb/qsambaclient/src/smbobject.cpp
    smb/qsambaclient/src/smbobject.h
    smb/smblocation.h
    smb/smblocation.cpp
    smb/smblocationauthentication.cpp
    smb/smblocationauthentication.h
    smb/qsambaclient/src/smbplaces.cpp
    smb/qsambaclient/src/smbplaces.h
    smb/qsambaclient/src/smbusershare.cpp
    smb/qsambaclient/src/smbusershare.h
)

# Find libsmbclient
if(ENABLE_SMBCLIENT)
    find_path(SAMBA_INCLUDE_DIR
              NAMES libsmbclient.h
              HINTS /usr/include/smbclient /usr/include/samba /usr/include/samba-3.0 /usr/include/samba-4.0)
    find_library(SAMBA_LIBRARIES NAMES smbclient)

    if(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
        message(STATUS "Found libsmbclient")
        set(SAMBA_FOUND TRUE)
        add_definitions(-DENABLE_SMBCLIENT=1)
    else()
        message(FATAL_ERROR "Could not find Samba libsmbclient")
    endif()
    mark_as_advanced(SAMBA_INCLUDE_DIR SAMBA_LIBRARIES)
else()
    set(SAMBA_FOUND FALSE)
    add_definitions(-DENABLE_SMBCLIENT=0)
endif()

if(SAMBA_FOUND)
    set(SOURCES ${SOURCES} ${SAMBA_SOURCES})
endif()

add_library(swordfishfoldersplugin SHARED ${SOURCES})
target_link_libraries(swordfishfoldersplugin
                      Qt5::Core
                      Qt5::Gui
                      Qt5::Widgets
                      Qt5::Qml
                      Qt5::Quick)

if(SAMBA_FOUND)
    include_directories(${SAMBA_INCLUDE_DIR})
    target_link_libraries(swordfishfolders ${SAMBA_LIBRARIES})
endif()

install(FILES qmldir
        DESTINATION ${QT_INSTALL_DIR}/Swordfish/Folders)
install(TARGETS swordfishfoldersplugin
        DESTINATION ${QT_INSTALL_DIR}/Swordfish/Folders)
