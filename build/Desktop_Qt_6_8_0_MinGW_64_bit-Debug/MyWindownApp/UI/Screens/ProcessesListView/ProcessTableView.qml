import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound


TableView {
    id: tableView
    width: parent.width
    height: parent.height
    required property var model
    required property var getColumnWidth

    model: model

    columnWidthProvider: function(column) {
        return getColumnWidth(column);
    }

    delegate: Rectangle {
        id: cellData

        property var model: tableView.model

        implicitHeight: 40
        color: "#f5f5f5"

        required property int row
        required property int column

        Text {
            anchors.centerIn: parent
            elide: Text.ElideRight
            text: {
                const roles = ["name", "cpu", "memory", "disk", "network"];
                if (!cellData.model) return "N/A";
                const idx = cellData.model.index(cellData.row, cellData.column);
                const roleName = roles[cellData.column];
                const roleId = cellData.model.roleForName(roleName);
                return cellData.model.data(idx, roleId);
            }
            font.pixelSize: 14
            color: "#333"
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
    }
}
