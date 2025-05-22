cmake_minimum_required(VERSION 3.16)

project(MyWindownApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

find_package(Qt6 6.5 REQUIRED COMPONENTS Charts)

find_package(Qt6 6.5 REQUIRED COMPONENTS Test)

# find_package(Qt6 6.5 REQUIRED COMPONENTS Graphs)


qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appMyWindownApp
    main.cpp
)

# Test executable
# add_executable(TestIviSocketServer
#     tests/test_IviSocketServer.cpp
#     core/ivisocketserver.cpp
# )

# target_include_directories(TestIviSocketServer PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/core
# )

qt_policy(SET QTP0004 NEW)

qt_add_qml_module(appMyWindownApp
    URI MyWindownApp
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES
        QML_FILES UI/Components/NavigationBar.qml
        QML_FILES UI/Screens/Performance/PerformanceView.qml
        QML_FILES
        QML_FILES
        SOURCES
        QML_FILES
        QML_FILES UI/Screens/ProcessesListView/ProcessesListView.qml
        QML_FILES UI/Screens/ProcessesListView/HeaderView.qml
        QML_FILES
        QML_FILES UI/Screens/ProcessesListView/ColumnUtils.qml
        QML_FILES
        QML_FILES UI/Screens/ProcessesListView/ComponentTable.qml
        SOURCES core/systemmonitor.h core/systemmonitor.cpp
        SOURCES model/systemstats.h model/systemstats.cpp
        SOURCES model/cpucore.h model/cpucore.cpp
        SOURCES model/cpugeneral.h model/cpugeneral.cpp
        SOURCES model/systemcpu.h model/systemcpu.cpp
        SOURCES model/systemmem.h model/systemmem.cpp
        SOURCES core/ISystemMonitor.h
        SOURCES model/processinfo.h model/processinfo.cpp
        SOURCES viewmodel/systemstatsviewmodel.h viewmodel/systemstatsviewmodel.cpp
        SOURCES core/ivisocketserver.h core/ivisocketserver.cpp
        SOURCES
        SOURCES viewmodel/cpustatsviewmodel.h viewmodel/cpustatsviewmodel.cpp
        SOURCES viewmodel/memstatsviewmodel.h viewmodel/memstatsviewmodel.cpp
        SOURCES core/dataprocessor.h core/dataprocessor.cpp
        RESOURCES
        SOURCES core/configmanager.h core/configmanager.cpp
        SOURCES
        QML_FILES UI/Screens/Performance/CpuChart.qml
        QML_FILES UI/Screens/Performance/MemChart.qml
        QML_FILES
        QML_FILES UI/Screens/Performance/TestCpuLineChart.qml
        QML_FILES UI/Screens/Performance/ComponentChartUpdater.qml
        SOURCES
        SOURCES viewmodel/processlistviewmodel.h viewmodel/processlistviewmodel.cpp
        QML_FILES
        QML_FILES UI/Screens/ProcessesListView/ColorUtils.qml
        QML_FILES UI/Screens/Performance/CustomLegend.qml
        RESOURCES assets/icon/file.png assets/icon/list.png assets/icon/settings.png assets/icon/speedometer.png
        RESOURCES assets/icon/history.svg assets/icon/list.svg assets/icon/performance.svg assets/icon/setting.svg
        QML_FILES UI/Screens/Performance/ComponentTitleBar.qml
        SOURCES thread/threadmanager.h thread/threadmanager.cpp
        SOURCES core/overloaddetector.h core/overloaddetector.cpp
        SOURCES core/IProcessManager.h
        SOURCES core/processmanager.h core/processmanager.cpp
        QML_FILES UI/Screens/StressTest/StressTestView.qml
        QML_FILES
        QML_FILES UI/Screens/MainPerformance/MainPerformanceView.qml
        QML_FILES UI/Screens/StressTest/CustomLabel.qml
        QML_FILES UI/Components/CustomButton.qml
        SOURCES storage/csvwriter.h storage/csvwriter.cpp
        SOURCES storage/csvwritermanager.h storage/csvwritermanager.cpp
        SOURCES storage/datastorageworker.h storage/datastorageworker.cpp
        SOURCES model/OverloadSnapshot.h
        SOURCES setting/configkeys.h
        RESOURCES
        QML_FILES UI/Screens/SettingConfig/SettingView.qml
        QML_FILES UI/Screens/SettingConfig/ConfigField.qml
        SOURCES model/OverloadConfig.h
        RESOURCES config/Priority.txt config/WhileList.txt
        SOURCES model/ProcessSelectionResult.h
        QML_FILES UI/Screens/Popup/PopupView.qml
        SOURCES viewmodel/configviewmodel.h viewmodel/configviewmodel.cpp

        # QML_FILES UI/Screens/Processes/ProcessView.qml
        # QML_FILES UI/Screens/ResourcesView.qml
        # QML_FILES UI/Screens/HistoryView.qml

)

add_test(NAME IviSocketServerTest COMMAND TestIviSocketServer)

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
        Qt6::Test
        Qt6::Network
        # Qt6::Graphs
)


include(GNUInstallDirs)
install(TARGETS appMyWindownApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
