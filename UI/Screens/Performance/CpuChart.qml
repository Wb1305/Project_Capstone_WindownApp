import QtQuick
import QtCharts

pragma ComponentBehavior: Bound


ChartView {
    id: cpuChart

    required property var seriesCpuModel
    property alias xAxis: xAxis
    property alias yAxis: yAxis
    property int maxSeconds: 60

    antialiasing: true
    legend.visible: true
    legend.alignment: Qt.AlignBottom
    // theme: ChartView.ChartThemeDark

    ValueAxis {
        id: xAxis
        min: 0
        max: cpuChart.maxSeconds
        tickCount: 7
        labelFormat: "%d secs"
    }

    ValueAxis {
        id: yAxis
        min: 0
        max: 100
        labelFormat: "%d %%"
        tickCount: 6
    }

    // LineSeries{
    //     name: "CPU"
    //     axisX: xAxis
    //     axisY: yAxis
    // }

    // Tạo LineSeries động theo số core
    ComponentChartUpdater{
        chartView: cpuChart
        viewModel: cpuChart.seriesCpuModel.cpu
        maxSeconds: cpuChart.maxSeconds
    }
}
