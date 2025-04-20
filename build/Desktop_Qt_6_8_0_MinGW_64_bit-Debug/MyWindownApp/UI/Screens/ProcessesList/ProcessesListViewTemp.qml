import QtQuick
import QtQuick.Controls

pragma ComponentBehavior: Bound

TableView {
    id: viewProcList

    required property var procModel
    anchors.fill: parent
    model: procModel
    clip: true
    columnSpacing: 1
    rowSpacing: 1

    delegate: Rectangle {
        height: 40
        border.width: 1
        // Cột được xác định qua styleData.column. Bạn có thể dùng các điều kiện để hiển thị nội dung
        Text {
            anchors.centerIn: parent
            text: {
                // Sử dụng styleData.column để chọn role tương ứng
                if (styleData.column === 0)
                    return model.pid;
                else if (styleData.column === 1)
                    return model.name;
                else if (styleData.column === 2)
                    return model.user;
                else if (styleData.column === 3)
                    return model.cpu;
                else if (styleData.column === 4)
                    return model.mem;
                else if (styleData.column === 5)
                    return model.memMB;
                else if (styleData.column === 6)
                    return model.timestamp;
                else
                    return "";
            }
        }
    }

    // Delegate cho header của mỗi cột
    columnHeaderDelegate: Rectangle {
        height: 40
        border.width: 1
        color: "#e0e0e0"
        Text {
            anchors.centerIn: parent
            text: {
                // Tùy chỉnh tiêu đề cho từng cột
                if (styleData.column === 0)
                    return "PID";
                else if (styleData.column === 1)
                    return "Tên tiến trình";
                else if (styleData.column === 2)
                    return "User";
                else if (styleData.column === 3)
                    return "CPU (%)";
                else if (styleData.column === 4)
                    return "Mem (%)";
                else if (styleData.column === 5)
                    return "Mem (MB)";
                else if (styleData.column === 6)
                    return "Timestamp";
                else
                    return "";
            }
            font.bold: true
        }
    }

    // Bạn có thể tinh chỉnh thêm kích thước cột thông qua columnWidths property
    // Ví dụ: Đặt chiều rộng từng cột nếu cần (theo thứ tự 0 đến 6)
    columnWidths: [
        60,    // Cột PID
        200,   // Cột Tên tiến trình
        150,   // Cột User
        80,    // Cột CPU (%)
        80,    // Cột Mem (%)
        80,    // Cột Mem (MB)
        150    // Cột Timestamp
    ]
}
