import QtQuick
import QtCharts
import QtQuick.Layouts

pragma ComponentBehavior: Bound


ChartView {
    id: memChart
    property var seriesMemModel
    property alias xAxis: xAxis
    property alias yAxis: yAxis
    property int maxSeconds: 60

    antialiasing: true
    legend.visible: true
    legend.alignment: Qt.AlignBottom


    ValueAxis {
        id: xAxis
        min: 0
        max: memChart.maxSeconds
        tickCount: 7
        labelFormat: "%d secs"
    }

    ValueAxis {
        id: yAxis
        min: 0
        max: 100
        tickCount: 6
        labelFormat: "%d %%"
    }

    // Tạo LineSeries mem (ram - swap)
    ComponentChartUpdater{
        id: componentChart
        chartView: memChart
        viewModel: memChart.seriesMemModel.mem
        maxSeconds: memChart.maxSeconds
    }
}

