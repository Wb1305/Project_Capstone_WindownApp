
import QtQuick

QtObject {
    required property int columnCount
    required property int maxWidth
    property var ratios: [3, 1, 1, 1, 1, 1, 1]  // tổng =
    property var mpadding: 20

    // Tổng tỉ lệ
    property int totalRatio: {
        var sum = 0;
        for (let i = 0; i < columnCount; ++i)
            sum += (ratios[i] || 1);
        return sum;
    }

    function getColumnWidth(index) {
        const ratio = ratios[index] || 1;
        return Math.floor((maxWidth - mpadding) * ratio / totalRatio);
    }

    function getTotalColumnWidth() {
        return maxWidth - mpadding;  // toàn bộ chiều rộng hiện có
    }
}
