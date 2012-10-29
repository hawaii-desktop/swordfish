include_directories(
    ${VibeGui_INCLUDE_DIRS}
    ${VibeWidgets_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(${VibeWidgets_DEFINITIONS})

set(SOURCES
    main.cpp
    mainwindow.cpp
    filemanager.cpp
    fileviewcontroller.cpp
    filecopyjob.cpp
    filemovejob.cpp
    directorycopyjob.cpp
    directorymovejob.cpp
    directorysizejob.cpp
)

set(UI_SOURCES
    mainwindow.ui
)

qt5_wrap_ui(UI_FILES ${UI_SOURCES})

qt5_add_resources(RC_FILES swordfish.qrc)

add_executable(swordfish ${SOURCES} ${UI_FILES} ${RC_FILES})
set_target_properties(swordfish PROPERTIES COMPILE_FLAGS ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS})
target_link_libraries(swordfish ${VibeGui_LIBRARIES} ${VibeWidgets_LIBRARIES})

install(TARGETS swordfish DESTINATION ${CMAKE_INSTALL_BINDIR})
