include_directories(
    disk
    trash   
    net
    smb 
    smb/qsambaclient/src
)

set(PLUGIN_DIR Swordfish/Folders)

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
    net/netauthenticationdata.cpp
    net/netauthenticationdata.h  
    net/netutil.cpp
    net/netutil.h
)

add_library(swordfishfoldersplugin SHARED ${SOURCES})
target_link_libraries(swordfishfoldersplugin
                      Qt5::Core
                      Qt5::Gui
                      Qt5::Widgets
                      Qt5::Qml
                      Qt5::Quick)

## samba requires libsmbclient
find_path(SAMBA_INCLUDE_DIR 
          NAMES libsmbclient.h 
          HINTS /usr/include/smbclient /usr/include/samba /usr/include/samba-3.0 /usr/include/samba-4.0)
find_library(SAMBA_LIBRARIES NAMES smbclient)

if(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
   message(STATUS "Found samba: include=${SAMBA_INCLUDE_DIR}  library=${SAMBA_LIBRARIES}")
   INCLUDE_DIRECTORIES(${SAMBA_INCLUDE_DIR})
   TARGET_LINK_LIBRARIES(swordfishfolders ${SAMBA_LIBRARIES})
else(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
   message(FATAL_ERROR "Could not find Samba libsmbclient")
endif(SAMBA_INCLUDE_DIR AND SAMBA_LIBRARIES)
mark_as_advanced(SAMBA_INCLUDE_DIR SAMBA_LIBRARIES)
## end samba confiuration

install(FILES qmldir
        DESTINATION ${QT_INSTALL_DIR}/Swordfish/Folders)
install(TARGETS swordfishfoldersplugin
        DESTINATION ${QT_INSTALL_DIR}/Swordfish/Folders)
