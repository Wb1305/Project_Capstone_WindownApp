import QtQuick
// import QtQuick.Controls

pragma ComponentBehavior: Bound

                          TableView {
    id: tableViewData
    required property var model
    required property var columnUtils

    model: model

    width: columnUtils.getTotalColumnWidth()
    height: columnUtils.getTotalColumnWidth()

    columnWidthProvider: function(column) {
        return columnUtils.getColumnWidth(column);
    }
    delegate: Rectangle {
        id: comp
        required property int row
        required property int column
        height: 35
        color: "#f5f5f5"
        // Component.onCompleted: {
        //     console.log("Gọi bên trong TableView -> delegate");
        //     if (tableViewData.model) {
        //         console.log("Count Column: " + tableViewData.model.columnCount());
        //         for (var row = 0; row < tableViewData.model.rowCount(); row++) {
        //             console.log("Name:", tableViewData.model.getData(row, "name"));
        //             console.log("CPU:", tableViewData.model.getData(row, "cpu"));
        //             console.log("Memory:", tableViewData.model.getData(row, "memory"));
        //         }
        //     } else {
        //         console.warn("Model chưa được khởi tạo!");
        //     }
        // }

        // Component.onCompleted: {
        //     console.log("Delegate created - row:", row, "column:", column);
        //     console.log("modelData:", modelData);  // ✅ Kiểm tra dữ liệu modelData
        // }

        Text {
            anchors.centerIn: parent
            elide: Text.ElideRight
            text: {
                const roles = ["name", "cpu", "memory", "disk", "network"];
                if (!tableViewData.model) return "N/A";
                const idx = tableViewData.model.index(comp.row, comp.column);
                const roleName = roles[comp.column];
                const roleId = tableViewData.model.roleForName(roleName);
                return tableViewData.model.data(idx, roleId);
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
