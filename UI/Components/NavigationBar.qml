import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


pragma ComponentBehavior: Bound


TabBar {
    id: tabBar
    Layout.alignment: Qt.AlignHCenter  //Canh giữa
    Layout.topMargin: 10
    Layout.bottomMargin: 10
    Layout.preferredWidth: parent.width / 3

    background: Rectangle { color: "white" }

    Repeater {
        model: ["Processes", "Performance", "History"]
        TabButton {
            id: tabButton
            required property string modelData
            required property int index
            text: modelData

            contentItem: Text {
                text: tabButton.text
                color: "black"
                font.bold: true
                anchors.centerIn: parent
            }

            checkable: true
            background: Rectangle {
                color: tabBar.currentIndex === tabButton.index ? "lightgray" : "transparent"
            }
        }
    }
}
