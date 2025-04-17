import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
// import QtCharts
pragma ComponentBehavior: Bound



ScrollView{
    id: scrollItem
    // Ẩn scrollbar
    ScrollBar.horizontal.policy: ScrollBar.AsNeeded
    ScrollBar.vertical.policy: ScrollBar.AsNeeded
    // ScrollBar.vertical.policy: ScrollBar.AlwaysOff
    // ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

    clip: true

    required property var performModel
    required property var preferHeight
    required property var preferWidth

    ColumnLayout{
        id: perfView
        spacing: 0

        Layout.fillWidth: true
        Layout.fillHeight: true

        property var performModel: scrollItem.performModel
        property var preferHeight: scrollItem.preferHeight
        property var preferWidth: scrollItem.preferWidth

        ComponentTitleBar{
            label: "CPU"
            Layout.preferredWidth: 50
        }

        CpuChart {
            Layout.fillWidth: true
            Layout.preferredHeight: perfView.preferHeight
            Layout.preferredWidth: perfView.preferWidth
            seriesCpuModel: perfView.performModel
        }

        ComponentTitleBar{
            label: "Legend of CPU"
            color: "red"
            Layout.fillWidth: true
            visible: false
        }

        ComponentTitleBar{
            label: "Memmory and Swap"
        }

        MemChart {
            Layout.fillWidth: true
            Layout.preferredHeight: perfView.preferHeight
            Layout.preferredWidth: perfView.preferWidth
            seriesMemModel: perfView.performModel
        }

        ComponentTitleBar{
            label: "Legend of Mem"
            color: "red"
            Layout.fillWidth: true
            Layout.preferredHeight: 70
            visible: false
        }
    }
}


