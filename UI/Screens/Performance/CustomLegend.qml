import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


pragma ComponentBehavior: Bound

Rectangle {
    id: customLegend
    property var seriesList          // List<QAbstractSeries>
    property var getDetailFn        // function(name) => detail string

    Flow {
        id: flowLayout
        anchors.fill: parent
        spacing: 16
        flow: Flow.LeftToRight

        Repeater {
            model: customLegend.seriesList ?? []
            delegate: Row {
                spacing: 8
                width: implicitWidth
                height: Math.max(iconLoader.implicitHeight, labelCol.implicitHeight)

                Loader {
                    id: iconLoader
                    width: 24; height: 24
                    sourceComponent: modelData.name.startsWith("RAM") ? ramIcon : swapIcon
                }

                Column {
                    id: labelCol
                    spacing: 2
                    Text {
                        text: modelData.name.startsWith("RAM") ? "Memory" : modelData.name.startsWith("SWAP") ? "Swap" : modelData.name
                        font.pixelSize: 13
                        font.bold: true
                    }
                    Text {
                        text: typeof(customLegend.getDetailFn) === "function" ? customLegend.getDetailFn(modelData.name) : ""
                        font.pixelSize: 11
                        color: "#555"
                        wrapMode: Text.Wrap
                    }
                }
            }
        }
    }

    // RAM icon (gauge style)
    Component {
        id: ramIcon
        Item {
            width: 20; height: 20
            Canvas {
                anchors.fill: parent
                onPaint: {
                    let ctx = getContext("2d")
                    ctx.clearRect(0, 0, width, height)
                    ctx.beginPath()
                    ctx.arc(width/2, height/2, 8, 0, 2 * Math.PI)
                    ctx.fillStyle = "#eee"
                    ctx.fill()

                    ctx.beginPath()
                    ctx.moveTo(width/2, height/2)
                    ctx.arc(width/2, height/2, 8, -Math.PI / 2, Math.PI / 2, false)
                    ctx.closePath()
                    ctx.fillStyle = "crimson"
                    ctx.fill()
                }
            }
        }
    }

    // SWAP icon (square style)
    Component {
        id: swapIcon
        Rectangle {
            width: 12; height: 12
            radius: 2
            color: "#4CAF50"
            border.color: "black"
        }
    }
}
