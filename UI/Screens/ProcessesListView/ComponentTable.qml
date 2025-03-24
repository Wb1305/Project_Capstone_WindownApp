import QtQuick

Rectangle {
    id: rectComp
    implicitHeight: 40
    color: "#f5f5f5"

    required property int rowCount
    required property int columnCount
    required property var model

    readonly property var roles: ["name", "pid", "cpu", "memory", "disk", "network"]

    // Component.onCompleted: {
    //     console.log("Gọi bên trong TableView");
    //     console.log("Delegate created - row:", rowCount, "column:", columnCount);
    //     console.log("modelData:", model);
    //     if (model) {
    //         console.log("Count Column: " + model.columnCount());
    //         for (var row = 0; row < model.rowCount(); row++) {
    //             console.log("Name:", model.getData(row, "name"));
    //             console.log("CPU:", model.getData(row, "cpu"));
    //             console.log("Memory:", model.getData(row, "memory"));
    //         }
    //     } else {
    //         console.warn("Model chưa được khởi tạo!");
    //     }
    // }

    Text {
        anchors.centerIn: parent
        elide: Text.ElideRight
        font.pixelSize: 14
        color: "#333"
        text: {
            // const roles = ["name", "pid", "cpu", "memory", "disk", "network"];
            if (!rectComp.model) return "N/A"
            const idx = rectComp.model.index(rectComp.rowCount, rectComp.columnCount)
            const roleName = rectComp.roles[rectComp.columnCount] || ""
            const roleId = rectComp.model.roleForName(roleName)
            return rectComp.model.data(idx, roleId)
        }
    }

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
