import QtQuick

Rectangle {
    id: rectComp
    implicitHeight: 40
    color: "#f5f5f5"

    required property int row
    required property int column
    required property var model

    Text {
        id: textcontent
        anchors.centerIn: parent
        font.pixelSize: 14
        color: "#333"

        property var value: rectComp.model.getData(rectComp.row, rectComp.model.roleNameAt(rectComp.column))

        text: {
            // console.log("[Cell]", "row:", rectComp.row, "column:", rectComp.column, "value:", textcontent.value)
            return textcontent.value !== undefined && textcontent.value !== null ? textcontent.value.toString() : "N/A"
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

