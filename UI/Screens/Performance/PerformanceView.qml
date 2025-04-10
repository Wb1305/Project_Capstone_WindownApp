import QtQuick
// import QtQuick.Controls
import QtQuick.Layouts
// import QtCharts
pragma ComponentBehavior: Bound

Rectangle {
    id: perfView
    required property var performModel
    // Layout.preferredWidth: 900
    // Layout.preferredHeight: 700

    ColumnLayout{
        Layout.fillWidth: true
        Layout.fillHeight: true
        // Layout.alignment: Qt.AlignHCenter
        spacing: 10
        Text { text: "CPU Usage"; font.pixelSize: 18; color: "black"; font.bold: true }

        CpuChart {
            // property var seriesModel
            Layout.preferredWidth: 800
            Layout.preferredHeight: 400
            // anchors.fill: parent
            seriesCpuModel: perfView.performModel
        }

        Text { text: "Memory Usage"; font.pixelSize: 18; color: "black"; font.bold: true }
        // Rectangle{
        //     Layout.fillWidth: true
        //     Layout.preferredWidth: 800
        //     Layout.preferredHeight: 250
        //     Layout.alignment: Qt.AlignVCenter
        //     color:"lightgreen"
        //     MemChart {
        //         Layout.preferredWidth: 800
        //         Layout.preferredHeight: parent.height
        //         seriesModel: perfView.systemStatsVM.mem.series
        //     }
        // }
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
}
