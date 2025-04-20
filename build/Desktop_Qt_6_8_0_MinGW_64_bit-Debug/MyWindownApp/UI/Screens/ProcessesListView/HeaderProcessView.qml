import QtQuick

pragma ComponentBehavior: Bound

Rectangle {
    id: headerRoot
    width: parent.width
    height: 40
    color: "#e0e0e0"

    required property var model
    required property var getColumnWidth

    Row {
        width: headerRoot.width
        height: parent.height

        Repeater {
            model: model ? model.columnCount() : 0

            Rectangle {
                id: cellHeader
                required property var model
                required property int index
                width: headerRoot.getColumnWidth(index)
                height: parent.height
                color: "#d0d0d0"

                Text {
                    text: cellHeader.model.header(cellHeader.index)
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
