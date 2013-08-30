include_directories(
    ${CMAKE_SOURCE_DIR}/3rdparty/kommodity/library/gio
    ${CMAKE_SOURCE_DIR}/3rdparty/kommodity/include
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_definitions(-DQT_PLUGIN)

set(SOURCES
    placesmodel.cpp
    placesitem.cpp
    bookmarkitem.cpp
    volumeitem.cpp
    swordfishplugin.cpp
    foldermodel.cpp
    folderitem.cpp
)

add_library(declarative_swordfish SHARED ${SOURCES})
qt5_use_modules(declarative_swordfish Qml Quick)
target_link_libraries(declarative_swordfish kommoditygio)

# Install plugin
install(FILES qmldir
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/hawaii/qml/Swordfish)
install(TARGETS declarative_swordfish
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/hawaii/qml/Swordfish)

# QML and JavaScript code
add_subdirectory(qml)
