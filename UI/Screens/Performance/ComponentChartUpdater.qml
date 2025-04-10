
import QtQuick
import QtCharts

Item {
    id: updater

    required property var chartView        // ChartView cần cập nhật
    required property var viewModel        // SystemStatsVM.mem
    required property int maxSeconds
    property var seriesList: []
    property bool initialized: false

    Connections {
        target: updater.viewModel
        function onSeriesChanged() {
            console.log("===Component===");
            let cntSeries = updater.viewModel.series.length;
            if (!updater.initialized && cntSeries > 0) {
                for (let i = 0; i < cntSeries; ++i) {
                    let s = updater.chartView.createSeries(ChartView.SeriesTypeLine, "LineChart", updater.chartView.xAxis, updater.chartView.yAxis);
                    // updater.chartView.seriesList.push(s);
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
                    console.log("Series", i, "append:", pt.x, pt.y);
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
