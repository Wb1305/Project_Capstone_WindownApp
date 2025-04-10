import QtQuick
import QtCharts
// import QtQuick.Controls

pragma ComponentBehavior: Bound


//=============
Rectangle {
    id: coreLineChart

    property int maxPoints: 60
    property int coreCount: 8
    property var coreSeries: []  // lưu danh sách LineSeries

    ChartView {
        id: cpuChart
        anchors.fill: parent
        antialiasing: true
        legend.visible: true
        theme: ChartView.ChartThemeDark

        ValueAxis {
            id: xAxis
            min: 0
            max: coreLineChart.maxPoints
            tickCount: 7
            titleText: "Time (s)"
        }

        ValueAxis {
            id: yAxis
            min: 0
            max: 100
            tickCount: 11
            titleText: "CPU Utilization (%)"
        }
    }

    // Tạo các LineSeries động khi bắt đầu
    Component.onCompleted: {
        for (let i = 0; i < coreCount; ++i) {
            let series = cpuChart.createSeries(ChartView.SeriesTypeLine, "Core " + (i + 1), xAxis, yAxis);
            coreSeries.push(series);
        }
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            for (let i = 0; i < coreLineChart.coreSeries.length; ++i) {
                let series = coreSeries[i];
                let y = Number((Math.random() * 100).toFixed(2));

                if (series.count >= maxPoints) {
                    // Xoá điểm đầu tiên
                    series.remove(0);

                    // Dịch các điểm còn lại sang trái
                    let shiftedPoints = [];
                    for (let j = 0; j < series.count; ++j) {
                        let pt = series.at(j);
                        shiftedPoints.push(Qt.point(pt.x - 1, pt.y));
                    }

                    series.clear();
                    for (let pt of shiftedPoints)
                        series.append(pt.x, pt.y);

                    series.append(maxPoints - 1, y);
                } else {
                    series.append(series.count, y);
                }
            }
        }
    }
}
