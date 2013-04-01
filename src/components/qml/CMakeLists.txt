# Install QML and JavaScript code
set(MODULE_FILES
    PlacesView.qml
)
install(FILES ${MODULE_FILES}
    DESTINATION ${CMAKE_INSTALL_QMLDIR}/Swordfish)
