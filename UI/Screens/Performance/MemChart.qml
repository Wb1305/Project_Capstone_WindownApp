import QtQuick
import QtCharts

pragma ComponentBehavior: Bound

ChartView {
    id: memChart
    required property var seriesMemModel
    property alias xAxis: xAxis
    property alias yAxis: yAxis
    property int maxSeconds: 60

    antialiasing: true
    // theme: ChartView.ChartThemeDark
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
        chartView: memChart
        viewModel: memChart.seriesMemModel.mem
        maxSeconds: memChart.maxSeconds
    }

}
