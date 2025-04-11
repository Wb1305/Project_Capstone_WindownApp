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
        "cpu": "CPU(%)",
        "mem": "Ram(%)",
        "memMB": "Ram(MB)"
    }

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
                color: "#d0d0d0"

                // tiêu đề Column
                Text {
                    // text: headerColumn.model.getHeader(nameCellRow.index)
                    text: headerColumn.headerMap[headerColumn.model.roleNameAt(nameCellRow.index)] || headerColumn.model.roleNameAt(nameCellRow.index)
                    anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: 14
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


// Rectangle {
//                 width: columnUtils.getColumnWidth(index)
//                 height: 60
//                 color: "#eeeeee"
//                 border.color: "#ccc"

//                 Column {
//                     anchors.centerIn: parent
//                     spacing: 2

//                     // Dòng 1: thông số tổng
//                     Text {
//                         font.pixelSize: 14
//                         text: {
//                             let role = model.roleNameAt(index);
//                             if (role === "cpu") return "25%";
//                             else if (role === "mem") return "37%";
//                             else return "";
//                         }
//                         horizontalAlignment: Text.AlignHCenter
//                         width: parent.width
//                     }

//                     // Dòng 2: tên cột
//                     Text {
//                         font.pixelSize: 12
//                         text: headerMap[model.roleNameAt(index)] || model.roleNameAt(index)
//                         horizontalAlignment: Text.AlignHCenter
//                         width: parent.width
//                         font.bold: true
//                     }
//                 }
//             }
//         }

