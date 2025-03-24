cmake_minimum_required(VERSION 3.16)

project(MyWindownApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

find_package(Qt6 6.5 REQUIRED COMPONENTS Charts)


qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appMyWindownApp
    main.cpp
)

qt_policy(SET QTP0004 NEW)

qt_add_qml_module(appMyWindownApp
    URI MyWindownApp
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES UI/Screens/Processes/ProcessView.qml
        QML_FILES UI/Components/NavigationBar.qml
        QML_FILES UI/Screens/Performance/PerformanceView.qml
        QML_FILES UI/Screens/Processes/ProcessRow.qml
        QML_FILES UI/Screens/Processes/ProcessHeader.qml
        SOURCES BE/Processes/updateprocessesui.h BE/Processes/updateprocessesui.cpp
        QML_FILES
        QML_FILES UI/Screens/ProcessesListView/ProcessesListView.qml
        QML_FILES UI/Screens/ProcessesListView/HeaderView.qml
        QML_FILES
        QML_FILES UI/Screens/ProcessesListView/ColumnUtils.qml
        QML_FILES
        QML_FILES UI/Screens/ProcessesListView/ComponentTable.qml
        # QML_FILES UI/Screens/Processes/ProcessView.qml
        # QML_FILES UI/Screens/ResourcesView.qml
        # QML_FILES UI/Screens/HistoryView.qml

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMyWindownApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMyWindownApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMyWindownApp
    PRIVATE
        Qt6::Quick
        Qt6::Charts
        # Qt6::ChartsQml
)


include(GNUInstallDirs)
install(TARGETS appMyWindownApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
