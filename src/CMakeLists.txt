include_directories(
    ${CMAKE_SOURCE_DIR}/3rdparty/kommodity/library/gio
    ${CMAKE_SOURCE_DIR}/3rdparty/kommodity/include
)

# Don't cast QString from/to ascii
add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_USE_QSTRINGBUILDER
)

set(SOURCES
    main.cpp
)

set(QM_SOURCES
    translations/it.ts
)

qt5_add_translation(QM_FILES ${QM_SOURCES})
qt5_add_resources(RC_FILES swordfish.qrc)

add_executable(swordfish ${SOURCES} ${RC_FILES} ${QM_FILES})
qt5_use_modules(swordfish Widgets Qml Quick)

install(TARGETS swordfish
    DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${QM_FILES}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/swordfish/translations)

add_subdirectory(components)
