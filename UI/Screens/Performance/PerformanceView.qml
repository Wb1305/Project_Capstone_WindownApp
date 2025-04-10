import QtQuick
// import QtQuick.Controls
import QtQuick.Layouts
// import QtCharts
pragma ComponentBehavior: Bound


ColumnLayout{
    id: perfView
    spacing: 2

    required property var performModel
    required property var setHeight

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: cpuText.implicitHeight + 4
        border.color: "red"
        Layout.leftMargin: 20
        Layout.rightMargin: 20
        Text {
            id: cpuText
            text: "CPU"
            font.pixelSize: 14
            font.bold: true
        }
    }

    CpuChart {
        Layout.fillWidth: true
        // Layout.preferredHeight: 300
        Layout.preferredHeight: perfView.setHeight
        Layout.minimumWidth: 850
        seriesCpuModel: perfView.performModel
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: memText.implicitHeight + 4
        border.color: "red"
        Layout.leftMargin: 20
        Layout.rightMargin: 20

        Text {
            id: memText
            text: "Memmory and Swap"
            font.pixelSize: 14
            font.bold: true
        }
    }

    MemChart {
        Layout.fillWidth: true
        Layout.preferredHeight: perfView.setHeight
        Layout.minimumWidth: 850
        seriesMemModel: perfView.performModel
    }
}





// Connections {
//     target: perfView.performModel.cpu
//     function onSeriesChanged() {
//         console.log("Series length = ", perfView.performModel.cpu.series.length);
//         console.log("[Performance] Series updated in QML");

//         for (let i = 0; i < perfView.performModel.cpu.series.length; ++i) {
//             let s = perfView.performModel.cpu.series[i];
//             console.log("Series:", s.name);
//             let pt = perfView.performModel.cpu.getLatestPointForCore(i);
//             console.log("New Point: x =", pt.x, ", y =", pt.y);
//         }
//     }
// }

