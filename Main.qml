import QtQuick
// import QtQuick.Controls
import QtQuick.Layouts
// import QtCharts
// import "./UI"
// import "./UI/Screens/Processes"
import "./UI/Screens/ProcessesListView"
import "./UI/Screens/Performance"
import "./UI/Components"


pragma ComponentBehavior: Bound


///-------------
Window {
    id: mainWindow

    width: 900
    height: 700
    visible: true
    title: qsTr("IVI System Monitor")


    NavigationBar{
        id: navigationBar
        width: parent.width
        height: 40
        anchors.topMargin: 10
        anchors.bottomMargin: 20
    }

    StackLayout {
        id: stackLayout
        // Layout.fillWidth: true
        // // Layout.fillHeight: true
        // Layout.topMargin: 10
        // currentIndex: navigationBar.currentIndex
        anchors.top: navigationBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: navigationBar.currentIndex

        ProcessesListView {
            id: processesListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            // model: mainWindow.processModel
            model: ProcessModel
        }

        PerformanceView {
            id: performanceView
            Layout.fillWidth: true
            Layout.fillHeight: true
            performModel: SystemStatsVM
        }

        // TestCpuLineChart{
        //     id: testCpuChart
        //     Layout.fillWidth: true
        //     Layout.fillHeight: true
        // }
    }





    // Connections {
    //     target: SystemStatsVM.cpu
    //     function onSeriesChanged() {
    //         console.log("Series length = ", SystemStatsVM.cpu.series.length);
    //         console.log("[Main] Series updated in QML");

    //         for (let i = 0; i < SystemStatsVM.cpu.series.length; ++i) {
    //             let s = SystemStatsVM.cpu.series[i];
    //             console.log("Series:", s.name);
    //             // let points = SystemStatsVM.cpu.getPointsForCore(i);
    //             // for (let i = 0; i < points.length; ++i) {
    //             //     let pt = points[i];
    //             //     console.log("Point", i, ": x =", pt.x, ", y =", pt.y);
    //             // }
    //             let pt = SystemStatsVM.cpu.getLatestPointForCore(i);
    //             console.log("New Point: x =", pt.x, ", y =", pt.y);
    //         }
    //     }
    // }
    // }
}
