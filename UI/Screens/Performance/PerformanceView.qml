import QtQuick
// import QtQuick.Controls
import QtCharts
Rectangle {
    width: 600
    height: 300
    color: "#1e1e1e"
    radius: 12
    border.color: "#4a4a4a"
    border.width: 1
    ChartView {
        title: "Biểu đồ Đường"
        anchors.fill: parent
        antialiasing: true

        LineSeries {
            name: "Dữ liệu"
            XYPoint { x: 0; y: 0 }
            XYPoint { x: 1; y: 2 }
            XYPoint { x: 2; y: 3 }
            XYPoint { x: 3; y: 2.5 }
            XYPoint { x: 4; y: 4 }
        }
    }
}
