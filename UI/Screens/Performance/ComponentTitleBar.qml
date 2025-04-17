import QtQuick
import QtQuick.Layouts

Rectangle {
    id: titleBar

    property bool toggled: true

    Layout.leftMargin: 20
    Layout.rightMargin: 20

    Layout.preferredHeight: titleText.implicitHeight + 10
    Layout.preferredWidth: titleText.implicitWidth + 50
    // color: "transparent"
    // border.color: "red"
    // visible: toggled ? true : false

    required property string label

    Text {
        id: titleText
        text: titleBar.label
        font.pixelSize: 14
        font.bold: true
        anchors.verticalCenter: parent.verticalCenter
    }

    // MouseArea{
    //     id: mouseArea
    //     anchors.fill: parent
    //     onClicked: titleBar.toggled = !titleBar.toggled
    // }
}
