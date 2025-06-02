import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


pragma ComponentBehavior: Bound


//=============

Rectangle {
    property alias currentIndex: tabBar.currentIndex

    TabBar {
        id: tabBar
        anchors.centerIn: parent
        width: parent.width * 0.5
        height: parent.height
        background: Rectangle { color: "transparent" }

        Repeater {
            model: ["Processes", "Performance", /*"History",*/ "Setting"]
            TabButton {
                id: tabButton

                required property string modelData
                required property int index
                text: modelData

                contentItem: RowLayout {
                    spacing: 5
                    anchors.centerIn: parent

                    Item {
                        Layout.preferredWidth: 12  // Padding trái
                    }

                    Image {
                        source: {
                            if (tabButton.modelData === "Processes") return "/qt/qml/MyWindownApp/assets/icon/list.svg"
                            if (tabButton.modelData === "Performance") return "/qt/qml/MyWindownApp/assets/icon/performance.svg"
                            if (tabButton.modelData === "History") return "/qt/qml/MyWindownApp/assets/icon/history.svg"
                            if (tabButton.modelData === "Setting") return "/qt/qml/MyWindownApp/assets/icon/setting.svg"
                            return "/qt/qml/MyWindownApp/assets/icon/setting.svg"
                        }
                        // width: 25
                        // height: 25
                        Layout.preferredWidth: 25
                        Layout.preferredHeight: 25
                        // Layout.alignment: Qt.AlignLeft

                        fillMode: Image.PreserveAspectFit
                    }

                    Text {
                        text: tabButton.modelData
                        color: "black"
                        font.bold: true
                        font.pixelSize: 15
                        verticalAlignment: Text.AlignVCenter
                        // Layout.alignment: Qt.AlignRight
                    }

                    Item {
                        Layout.preferredWidth: 12  // Padding phải
                    }
                }

                checkable: true
                background: Rectangle {
                    radius: 4
                    color: tabBar.currentIndex === tabButton.index ? "#dddddd" : "transparent"
                }
            }
        }
    }
}


