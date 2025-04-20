import QtQuick
// import QtQuick.Controls
import QtQuick.Layouts
// import QtCharts
import "./UI/Screens/ProcessesListView"
import "./UI/Screens/Performance"
import "./UI/Components"
import "./UI/Screens/MainPerformance"

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

        // PerformanceView {
        //     id: performanceView
        //     Layout.fillWidth: true
        //     Layout.fillHeight: true
        //     performModel: SystemStatsVM
        //     preferHeight: parent.height * 0.45
        //     preferWidth: parent.width
        // }

        // TestCpuLineChart{
        //     id: testCpuChart
        //     Layout.fillWidth: true
        //     Layout.fillHeight: true
        // }

        MainPerformanceView{
            // id: mainPerform
            Layout.fillWidth: true
            Layout.fillHeight: true
            performModel: SystemStatsVM
            preferHeight: parent.height * 0.45
            preferWidth: parent.width
            parentHeight: parent.height
        }
    }
}
