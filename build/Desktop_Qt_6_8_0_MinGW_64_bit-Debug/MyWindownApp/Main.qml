import QtQuick
// import QtQuick.Controls
import QtQuick.Layouts
// import QtCharts
import "./UI/Screens/ProcessesListView"
import "./UI/Screens/Performance"
import "./UI/Screens/MainPerformance"
import "./UI/Screens/SettingConfig"
import "./UI/Screens/Popup"
import "./UI/Components"

pragma ComponentBehavior: Bound


///-------------
Window {
    id: mainWindow

    width: 1200
    height: 650
    visible: true
    title: qsTr("IVI System Monitor")


    NavigationBar{
        id: navigationBar
        width: parent.width
        height: 50
        anchors.topMargin: 20
        anchors.bottomMargin: 20
    }

    StackLayout {
        id: stackLayout
        anchors.top: navigationBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentIndex: navigationBar.currentIndex

        ProcessesListView {
            id: processesListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            procModel: ProcessListVM
            setHeight: mainWindow.height
            setWidth: mainWindow.width
        }

        MainPerformanceView{
            // id: mainPerform
            Layout.fillWidth: true
            Layout.fillHeight: true
            performModel: SystemStatsVM
            preferHeight: mainWindow.height * 0.45
            preferWidth: mainWindow.width
            parentHeight: mainWindow.height
        }

        PerformanceView {
            id: performanceView
            Layout.fillWidth: true
            Layout.fillHeight: true
            performModel: SystemStatsVM
            preferHeight: mainWindow.height * 0.45
            preferWidth: mainWindow.width
        }

        SettingView{
            Layout.fillWidth: true
            Layout.fillHeight: true
            preferWidth: mainWindow.width
            preferHeight: mainWindow.height
            configManager: ConfigManager
        }

    }

    // --- popup ---
    PopupView{
        id: popupView
        systemMonitorModel: SystemMonitor
        parentWidth: mainWindow.width
        parentHeight: mainWindow.height
    }

}
