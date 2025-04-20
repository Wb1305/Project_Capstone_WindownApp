import QtQuick 2.15
import QtQuick.Controls

Rectangle {
    height: 50
    color: "#E0E0E0"
    signal menuSelected(string page)

    Row {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "Processes"
            onClicked: menuSelected("Processes")
        }

        // Button {
        //     text: "System"
        //     onClicked: menuSelected("System")
        // }
    }
}
