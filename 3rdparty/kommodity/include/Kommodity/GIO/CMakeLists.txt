
SET(kommodity_gio_public_HDRS
AppInfo
AppLaunchContext
Cancellable
ContentType
Drive
Error
File
FileAttribute
FileAttributeMatcher
FileDevice
FileEnumerator
FileIcon
FileInfo
FileInputStream
FileMonitor
FileOutputStream
Icon
InputStream
IOError
LoadableIcon
Mount
MountOperation
OutputStream
Seekable
ThemedIcon
Volume
VolumeMonitor
WrappedObject
SharedWrappedObject
)

install( FILES  ${kommodity_gio_public_HDRS} 
DESTINATION ${KOMMODITY_INCLUDE_INSTALL_DIR}/GIO )

