
// import QtQuick

// pragma ComponentBehavior: Bound

//                           Rectangle {
//     id: procRoot
//     anchors.fill: parent
//     required property var model

//     // Hàm dùng chung cung cấp độ rộng cột
//     function getColumnWidth(column) {
//         switch(column) {
//         case 0: return 350;
//         case 1: return 100;
//         case 2: return 100;
//         case 3: return 100;
//         case 4: return 100;
//         default: return 100;
//         }
//     }

//     function getTotalColumnWidth() {
//         let total = 0;
//         for (let i = 0; i <= 4; i++) {
//             total += getColumnWidth(i);
//         }
//         return total;
//     }

//     Column {
//         anchors.fill: parent
//         spacing: 0

//         // Header ngoài TableView, khớp kích thước cột data
//         Rectangle {
//             color: "#e0e0e0"
//             height: 40
//             width: procRoot.getTotalColumnWidth() // Đồng bộ với TableView

//             Row {
//                 width: parent.width
//                 height: parent.height

//                 Repeater {
//                     model: procRoot.model ? procRoot.model.columnCount() : 0

//                     Rectangle {
//                         id: nameCellRow
//                         required property int index
//                         width: procRoot.getColumnWidth(index)
//                         height: parent.height
//                         color: "#d0d0d0"

//                         Text {
//                             text: procRoot.model.header(nameCellRow.index)
//                             anchors.centerIn: parent
//                             font.bold: true
//                         }

//                         // Viền phải
//                         Rectangle {
//                             width: 1
//                             anchors {
//                                 top: parent.top
//                                 bottom: parent.bottom
//                                 right: parent.right
//                             }
//                             color: "#bbb"
//                         }

//                         // Viền dưới
//                         Rectangle {
//                             height: 1
//                             anchors {
//                                 left: parent.left
//                                 right: parent.right
//                                 bottom: parent.bottom
//                             }
//                             color: "#bbb"
//                         }
//                     }
//                 }
//             }
//         }

//         // TableView bên dưới Header -> hiển thị dữ liệu
//         TableView {
//             id: tableViewData
//             width: procRoot.getTotalColumnWidth()
//             height: parent.height - 80 // Trừ đi header và phần màu đỏ
//             model: procRoot.model

//             // Sử dụng chung hàm columnWidthProvider
//             columnWidthProvider: function(column) {
//                 return procRoot.getColumnWidth(column);
//             }

//             Component.onCompleted: {
//                 console.log("Gọi bên trong TableView");
//                 if (model) {
//                     console.log("Count Column: " + model.columnCount());
//                     for (var row = 0; row < model.rowCount(); row++) {
//                         console.log("Name:", model.getData(row, "name"));
//                         console.log("CPU:", model.getData(row, "cpu"));
//                         console.log("Memory:", model.getData(row, "memory"));
//                     }
//                 } else {
//                     console.warn("Model chưa được khởi tạo!");
//                 }
//             }

//             delegate: Rectangle {
//                 id: rect2
//                 implicitHeight: 40
//                 color: "#f5f5f5"

//                 required property int row
//                 required property int column

//                 Text {
//                     anchors.centerIn: parent
//                     elide: Text.ElideRight
//                     text: {
//                         const roles = ["name", "cpu", "memory", "disk", "network"];
//                         if (!procRoot.model) return "N/A";  // Kiểm tra model trước khi truy cập
//                         const idx = procRoot.model.index(rect2.row, rect2.column);
//                         const roleName = roles[rect2.column];
//                         const roleId = procRoot.model.roleForName(roleName);
//                         return procRoot.model.data(idx, roleId);
//                     }
//                     font.pixelSize: 14
//                     color: "#333"
//                 }

//                 // Viền phải
//                 Rectangle {
//                     width: 1
//                     anchors {
//                         top: parent.top
//                         bottom: parent.bottom
//                         right: parent.right
//                     }
//                     color: "#bbb"
//                 }
//             }
//         }
//     }
// }



import QtQuick
import QtQuick.Layouts
pragma ComponentBehavior: Bound



                          Rectangle {
    id: procRoot
    anchors.fill: parent
    required property var model

    Component.onCompleted: {
        console.log("procRoot - width:", width, "height:", height);
    }

    ColumnUtils {
        id: columnUtils
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header ngoài TableView
        HeaderView {
            model: procRoot.model
            columnUtils: columnUtils
        }

        TableView {
            id: tableViewData
            implicitWidth: columnUtils.getTotalColumnWidth()
            implicitHeight: parent.height
            model: procRoot.model

            // Sử dụng chung hàm columnWidthProvider
            columnWidthProvider: function(column) {
                return columnUtils.getColumnWidth(column);
            }
            delegate: ComponentTable
            {
                required property int row
                required property int column
                rowCount: row
                columnCount: column
                model: tableViewData.model
            }

        }
    }
}
