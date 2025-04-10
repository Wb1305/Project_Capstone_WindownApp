import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


pragma ComponentBehavior: Bound


//=============

Rectangle {
    // color: "#f5f5f5"
    // radius: 6
    // border.color: "#cccccc"
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
                required property string modelData
                required property int index
                text: modelData

                contentItem: Text {
                    text: tabButton.modelData
                    color: "black"
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    // anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: 15
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


// TabBar {
//     id: tabBar
//     // Layout.alignment: Qt.AlignHCenter  //Canh giữa
//     // Layout.topMargin: 10
//     // Layout.bottomMargin: 10
//     // Layout.preferredWidth: parent.width / 3
//     height: 25
//     width: parent.width/3
//     anchors.horizontalCenter: parent.horizontalCenter

//     background: Rectangle { color: "white" }

//     Repeater {
//         model: ["Processes", "Performance", "History"]
//         TabButton {
//             id: tabButton
//             required property string modelData
//             required property int index
//             text: modelData

//             contentItem: Text {
//                 text: tabButton.text
//                 color: "black"
//                 font.bold: true
//                 anchors.centerIn: parent
//             }

//             checkable: true
//             background: Rectangle {
//                 color: tabBar.currentIndex === tabButton.index ? "lightgray" : "transparent"
//             }
//         }
//     }
// }

