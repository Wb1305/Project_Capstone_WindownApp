
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    property int preferredWidth: 300
    property int preferredHeight: 40
    property alias labelText: label.text
    property alias value: spin.value
    property int minValue: 0
    property int maxValue: 100
    property int step: 1

    width: preferredWidth
    height: preferredHeight

    anchors.leftMargin: 5
    anchors.rightMargin: 5

    RowLayout {
        spacing: 12
        anchors.fill: parent
        anchors.margins: 5

        Label {
            id: label
            // horizontalAlignment: Text.AlignRight
            font.pixelSize: 14
            font.bold: true
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
        }

        Item { Layout.fillWidth: true }

        SpinBox {
            id: spin
            from: root.minValue
            to: root.maxValue
            stepSize: root.step
            value: 0
            Layout.preferredWidth: 80
            font.pixelSize: 14
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            editable: true
            onValueChanged: {
                if (spin.value < spin.from) {
                    spin.value = spin.from;
                } else if (spin.value > spin.to) {
                    spin.value = spin.to;
                }
            }
        }
    }

    border.color: "gray"
    border.width: 1
    radius: 5
}
