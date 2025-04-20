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
    // property var seriesList: []

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


//     CustomLegend {
//         width: 900
//         height: 50
//         anchors.top: memChart.bottom
//         seriesList: componentChart.seriesList
//         getDetailFn: function(name) {
//             const mem = memChart.seriesMemModel.mem;
//             const formatSize = function(mb) {
//                 if (mb < 1.0)
//                     return "0 bytes";
//                 if (mb < 1024.0)
//                     return `${mb.toFixed(0)} MB`;
//                 return `${(mb / 1024).toFixed(1)} GB`;
//             }

//             if (name.startsWith("RAM")) {
//                 const used = mem.ramPercent * mem.totalRam / 100;
//                 const percent = mem.ramPercent.toFixed(1);
//                 const total = formatSize(mem.totalRam);
//                 /*const cache = formatSize(mem.cacheSizeMB);*/

//                 return `${formatSize(used)} (${percent}%) of ${total}`;
//             }

//             if (name.startsWith("SWAP")) {
//                 const used = mem.swapPercent * mem.totalSwap / 100;
//                 const percent = mem.swapPercent.toFixed(1);
//                 const total = formatSize(mem.totalSwap);
//                 return `${formatSize(used)} (${percent}%) of ${total}`;
//             }

//             return "";
//         }
//     }
// }
