import QtQuick

QtObject {
    required property int columnCount
    property int firstColumnWidth: 350
    property int otherColumnWidth: 100

    function getColumnWidth(index) {
        if (index === 0)
            return firstColumnWidth;
        else
            return otherColumnWidth;
    }

    function getTotalColumnWidth() {
        return firstColumnWidth + (columnCount - 1) * otherColumnWidth;
    }
}
