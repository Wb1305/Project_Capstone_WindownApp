import QtQuick


pragma ComponentBehavior: Bound

Rectangle {
    id: headerColumn
    required property var model
    required property var columnUtils
    property var headerMap: {
        "name": "Name",
        "pid": "PID",
        "user": "User",
        "cpu": "CPU",
        "mem": "Ram",
        "memMB": "Ram"
    }

    property double totalCpuUsage: model.totalCpuUsagePercent
    property double totalRamUsage: model.totalRamUsagePercent

    color: "#e0e0e0"
    height: 40
    width: columnUtils.getTotalColumnWidth() // Đồng bộ với TableView

    Row {
        width: parent.width
        height: parent.height

        Repeater {
            model: headerColumn.model ? headerColumn.model.columnCount : 0

            Rectangle {
                id: nameCellRow
                required property int index
                width: headerColumn.columnUtils.getColumnWidth(index)
                height: headerColumn.height
                color: headerMouseArea.containsMouse ? "#d0e8ff" : "#d0d0d0"

                // tiêu đề Column
                Text {
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                    font.bold: true
                    font.pixelSize: 14
                    text: {
                        const role = headerColumn.model.roleNameAt(nameCellRow.index);
                        const label = headerColumn.headerMap[role] || role;

                        // Ví dụ tạm thời - sau này có thể lấy từ ViewModel
                        let summary = "";
                        if (role === "cpu") summary = " (" + headerColumn.totalCpuUsage.toFixed(1) + "%)";
                        else if (role === "mem") summary = " (" + headerColumn.totalRamUsage.toFixed(1)  + "%)";
                        else if (role === "memMB") summary = " (16 MB)";
                        return label + summary ;
                    }
                }

                MouseArea {
                    id: headerMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                }

                // Viền phải
                Rectangle {
                    width: 1
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        right: parent.right
                    }
                    color: "#bbb"
                }
                // Viền dưới
                Rectangle {
                    height: 1
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    color: "#bbb"
                }
            }
        }
    }
}

