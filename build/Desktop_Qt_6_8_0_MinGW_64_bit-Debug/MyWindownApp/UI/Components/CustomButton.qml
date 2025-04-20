import QtQuick

// Định nghĩa button custom từ Rectangle
Rectangle {
    id: customButton
    width: 100
    height: 40
    color: mouseArea.containsMouse ? "lightblue" : "blue"
    radius: 12
    border.color: "#ccc"
    border.width: 1

    property var buttonName: "Name Button"

    // Text hiển thị trên button
    Text {
        anchors.centerIn: parent
        text: customButton.buttonName
        font.pixelSize: 14
        font.bold: true
        color: "white"
    }


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true


    }

    // Thêm hiệu ứng mượt mà cho sự thay đổi màu sắc của button
    Behavior on color {
        ColorAnimation {
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }
}
