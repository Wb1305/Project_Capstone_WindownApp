import QtQuick 6.5
import QtQuick.Layouts

pragma ComponentBehavior: Bound

RowLayout {
    id: rowItem
    required property string name
    // required property string status
    required property string cpu
    required property string memory
    required property string disk
    required property string network

    property var columnWidths: [350, 100, 100, 100, 100]

    Repeater {
        model: [rowItem.name, rowItem.cpu, rowItem.memory, rowItem.disk, rowItem.network]
        Rectangle {
            id: recRowItem
            required property int index
            required property string modelData
            width: rowItem.columnWidths[index]
            height: 30
            color: "#f5f5f5"
            Text {
                text: recRowItem.modelData
                anchors.centerIn: parent
            }
        }
    }
}


// import QtQuick 6.5
// import QtQuick.Layouts

// pragma ComponentBehavior: Bound

// Rectangle {
//     id: rowContainer
//     width: parent.width
//     height: 30
//     color: hovered ? "#cce8ff" : "#f5f5f5"

//     property bool hovered: false
//     required property string name
//     required property string cpu
//     required property string memory
//     required property string disk
//     required property string network

//     RowLayout {
//         id: rowItem
//         anchors.fill: parent


//         property var columnRatios: [0.4, 0.15, 0.15, 0.15, 0.15]
//         readonly property var columnWidths: [350, 100, 100, 100, 100]

//         Repeater {
//             model: [rowContainer.name, rowContainer.cpu, rowContainer.memory, rowContainer.disk, rowContainer.network]

//             Rectangle {
//                 id: rect
//                 required property int index
//                 required property string modelData

//                 width: Math.min(rowContainer.width * rowItem.columnRatios[index], rowItem.columnWidths)
//                 height: parent.height
//                 color: "transparent"

//                 Text {
//                     text: rect.modelData
//                     anchors.centerIn: parent
//                     elide: Text.ElideRight
//                 }
//             }
//         }
//     }

//     MouseArea {
//         anchors.fill: parent
//         hoverEnabled: true
//         onEntered: rowContainer.hovered = true
//         onExited: rowContainer.hovered = false
//     }
// }




