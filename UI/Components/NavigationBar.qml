import QtQuick
import QtQuick.Controls
// import QtQuick.Layouts


pragma ComponentBehavior: Bound


//=============

Rectangle {
    property alias currentIndex: tabBar.currentIndex

    TabBar {
        id: tabBar
        anchors.centerIn: parent
        width: parent.width * 0.4
        height: parent.height
        background: Rectangle { color: "transparent" }

        Repeater {
            model: ["Processes", "Performance", "History"]
            TabButton {
                id: tabButton
                // indicator: null
                // leftPadding: 0
                // rightPadding: 0
                // topPadding: 0
                // bottomPadding: 0

                required property string modelData
                required property int index
                text: modelData

                contentItem: Row {
                    spacing: 5
                    anchors.centerIn: parent

                    Image {
                        source: {
                            if (tabButton.modelData === "Processes") return "/qt/qml/MyWindownApp/assets/icon/list.svg"
                            if (tabButton.modelData === "Performance") return "/qt/qml/MyWindownApp/assets/icon/performance.svg"
                            if (tabButton.modelData === "History") return "/qt/qml/MyWindownApp/assets/icon/history.svg"
                            return "/qt/qml/MyWindownApp/assets/icon/setting.svg"
                        }
                        width: 25
                        height: 25
                        fillMode: Image.PreserveAspectFit
                    }

                    Text {
                        text: tabButton.modelData
                        color: "black"
                        font.bold: true
                        font.pixelSize: 15
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


