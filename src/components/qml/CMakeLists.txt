# Install QML and JavaScript code
set(MODULE_FILES
    PlacesView.qml
    FolderView.qml
)
install(FILES ${MODULE_FILES}
    DESTINATION ${QT_IMPORTS_DIR}/Swordfish)
