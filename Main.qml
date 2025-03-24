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
    width: 900
    height: 500
    visible: true
    title: qsTr("Task Manager")

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        NavigationBar{
            id: navigationBar
        }
        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10
            currentIndex: navigationBar.currentIndex

            ProcessesListView {
                id: processesListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: processModel
            }

            PerformanceView {
                id: performanceView
                Layout.fillWidth: true
                Layout.fillHeight: true

            }
        }
    }
}
