import QtQuick
import QtCharts

pragma ComponentBehavior: Bound

ChartView {
    id: memChart
    required property var seriesModel
    // anchors.fill: parent
    antialiasing: true
    theme: ChartView.ChartThemeDark
    legend.visible: true
    backgroundColor: 'blue'

    ValueAxis {
        id: xMemAxis
        min: 0
        max: 60
        titleText: "Time (s)"
    }

    ValueAxis {
        id: yMemAxis
        min: 0
        max: 100
        titleText: "Memory (%)"
    }

    // Gắn các series trực tiếp từ ViewModel
    // Component.onCompleted: {
    //     for (var i = 0; i < memChart.seriesModel.length; ++i) {
    //         var s = seriesModel[i]
    //         s.axisX = xAxis
    //         s.axisY = yAxis
    //         cpuChart.addSeries(s)
    //     }
    // }

    Repeater {
        model: memChart.seriesModel
        delegate: Item{
            LineSeries {
                // property var modelData
                axisX: xMemAxis
                axisY: yMemAxis
                // name: modelData.name
                name: "MEM"
                useOpenGL: true
            }
        }
    }
}
