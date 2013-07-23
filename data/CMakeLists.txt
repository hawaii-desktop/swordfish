configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/org.freedesktop.FileManager1.service.in"
    "${CMAKE_CURRENT_BINARY_DIR}/org.freedesktop.FileManager1.service"
    IMMEDIATE @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.freedesktop.FileManager1.service
    DESTINATION ${CMAKE_INSTALL_DATADIR}/dbus-1/services)

install(FILES swordfish.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
