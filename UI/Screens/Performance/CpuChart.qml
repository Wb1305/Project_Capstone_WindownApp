import QtQuick
import QtCharts 6.2

pragma ComponentBehavior: Bound


ChartView {
    id: cpuChart
    required property var seriesCpuModel
    anchors.fill: parent
    x: 10
    y: 100
    antialiasing: true
    theme: ChartView.ChartThemeDark
    legend.visible: true

    ValueAxis {
        id: xAxis
        min: 0
        max: cpuChart.maxSeconds
        titleText: "Time (s)"
        tickCount: 7
    }

    ValueAxis {
        id: yAxis
        min: 0
        max: 100
        titleText: "CPU (%)"
        // tickCount: 11
    }

    // Tạo LineSeries động theo số core
    property var seriesList: []
    property bool initialized: false
    property int maxSeconds: 60

    Connections {
        target: cpuChart.seriesCpuModel.cpu
        function onSeriesChanged() {
            console.log("===CPU===")
            // Khởi tạo LineSeries 1 lần duy nhất khi series có dữ liệu
            if (!cpuChart.initialized && cpuChart.seriesCpuModel.cpu.series.length > 0) {
                let coreCount = cpuChart.seriesCpuModel.cpu.series.length;
                console.log("===Auto Create Series ===");
                console.log("Số core:", coreCount);

                for (let i = 0; i < coreCount; ++i) {
                    let s = cpuChart.createSeries(ChartView.SeriesTypeLine, "Core " + i+1, xAxis, yAxis);
                    cpuChart.seriesList.push(s);
                }

                initialized = true;
            }

            // Append điểm mới
            for (let i = 0; i < cpuChart.seriesList.length; ++i) {
                let series = cpuChart.seriesList[i];
                let pt = cpuChart.seriesCpuModel.cpu.getLatestPointForCore(i);

                if (pt && pt.x == undefined && pt.y == undefined) {
                    console.log("Error: x, y underfined!");
                }

                series.name = cpuChart.seriesCpuModel.cpu.series[i].name;

                if (series.count < maxSeconds) {
                    // Giai đoạn chưa đủ điểm, thêm bình thường theo thứ tự
                    series.append(series.count, pt.y);
                } else {
                    // Dịch các điểm sang trái
                    for (let j = 0; j < series.count; ++j) {
                        let p = series.at(j);
                        series.replace(j, p.x - 1, p.y);
                    }

                    // Xoá các điểm có x < 0
                    while (series.count > 0 && series.at(0).x < 0) {
                        series.remove(0);
                    }

                    // Thêm điểm mới tại x = maxSeconds - 1
                    series.append(maxSeconds - 1, pt.y);
                }
            }
        }
    }

    // Connections {
    //     target: cpuChart.seriesCpuModel.cpu
    //     function onSeriesChanged() {
    //         console.log("Series length = ", cpuChart.seriesCpuModel.cpu.series.length);
    //         console.log("[CPU] Series updated in QML");

    //         for (let i = 0; i < cpuChart.seriesCpuModel.cpu.series.length; ++i) {
    //             let s = cpuChart.seriesCpuModel.cpu.series[i];
    //             console.log("Series:", s.name);
    //             let pt = cpuChart.seriesCpuModel.cpu.getLatestPointForCore(i);
    //             console.log("New Point: x =", pt.x, ", y =", pt.y);
    //         }
    //     }
    // }
}
