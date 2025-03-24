import QtQuick


pragma ComponentBehavior: Bound

Rectangle {
    id: headerRoot
    required property var model
    required property var columnUtils

    color: "#e0e0e0"
    height: 40
    width: columnUtils.getTotalColumnWidth() // Đồng bộ với TableView

    Row {
        width: parent.width
        height: parent.height

        Repeater {
            model: headerRoot.model ? headerRoot.model.columnCount() : 0

            Rectangle {
                id: nameCellRow
                required property int index
                width: headerRoot.columnUtils.getColumnWidth(index)
                height: headerRoot.height
                color: "#d0d0d0"

                // tiêu đề Column
                Text {
                    text: headerRoot.model.header(nameCellRow.index)
                    anchors.centerIn: parent
                    font.bold: true
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
