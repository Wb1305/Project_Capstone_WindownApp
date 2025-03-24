import QtQuick 6.5
import QtQuick.Layouts

pragma ComponentBehavior: Bound

RowLayout {
    id: headerRow
    Layout.fillWidth: true
    readonly property var headers: ["Name", "CPU", "Memory", "Disk", "Network"]
    readonly property var columnWidths: [350, 100, 100, 100, 100]

    Repeater {
        model: headerRow.headers
        Rectangle {
            id: recItem

            required property int index
            required property string modelData
            property bool hovered: false

            width: headerRow.columnWidths[index]
            height: 30
            Text {
                text: recItem.modelData
                anchors.centerIn: parent
                font.bold: true
            }
            color: hovered ? "lightblue" : "#ddd"
            border.color: "#cccccc"
            border.width: 1

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: recItem.hovered = true
                onExited: recItem.hovered = false
            }
        }
    }
}


