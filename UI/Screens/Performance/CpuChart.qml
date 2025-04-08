import QtQuick
import QtCharts

ChartView{
    anchors.fill: parent
    antialiasing: true
    legend.visible: true
    theme: ChartView.ChartThemeDark

    ValueAxis { id: axisX; min: 0; max: 60; titleText: "Time" }
    ValueAxis { id: axisY; min: 0; max: 100; titleText: "CPU %" }

    // Bind các series CPU từ statsVM
    series: SystemStatsVM.cpu.series

    Component.onCompleted: {
        for (let i = 0; i < series.length; ++i) {
            series[i].attachAxis(axisX);
            series[i].attachAxis(axisY);
        }
    }
}
