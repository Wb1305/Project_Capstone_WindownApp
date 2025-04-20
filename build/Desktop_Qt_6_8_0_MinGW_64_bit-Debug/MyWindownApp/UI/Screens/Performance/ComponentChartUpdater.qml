
import QtQuick
import QtCharts

Item {
    id: updater

    required property var chartView        // ChartView cần cập nhật
    required property var viewModel        // SystemStatsVM.mem
    required property int maxSeconds
    property var seriesList: []
    property bool initialized: false
    property var myColorList: [ "#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd", "#8c564b", "#e377c2", "#7f7f7f" ]

    Connections {
        target: updater.viewModel
        function onSeriesChanged() {
            // console.log("===Component===");
            let cntSeries = updater.viewModel.series.length;
            if (!updater.initialized && cntSeries > 0) {
                for (let i = 0; i < cntSeries; ++i) {
                    let s = updater.chartView.createSeries(ChartView.SeriesTypeLine, "LineChart", updater.chartView.xAxis, updater.chartView.yAxis);
                    s.color = updater.myColorList[i % updater.myColorList.length];
                    updater.seriesList.push(s);
                }
                updater.initialized = true;
            }
            for (let i = 0; i < updater.seriesList.length; ++i) {
                let series = updater.seriesList[i];
                let pt = updater.viewModel.getLatestPoint(i);
                if (pt && pt.x === undefined && pt.y === undefined)
                    return;

                series.name = updater.viewModel.series[i].name;

                if (series.count < updater.maxSeconds) {
                    series.append(series.count, pt.y);
                    // console.log("series", i+1, "append:", pt.x, pt.y);
                } else {
                    for (let j = 0; j < series.count; ++j) {
                        let p = series.at(j);
                        series.replace(j, p.x - 1, p.y);
                    }
                    while (series.count > 0 && series.at(0).x < 0)
                        series.remove(0);

                    series.append(updater.maxSeconds - 1, pt.y);
                }
            }
        }
    }
}
