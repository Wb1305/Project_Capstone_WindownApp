import QtQuick 2.15
import QtQuick.Controls

Label {
    id: customLabel
    font.pixelSize: 14
    font.bold: true
    color: "White"
    padding: 5
    property var labelText: text

    text: customLabel.labelText
}
