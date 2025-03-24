import QtQuick
// import QtQuick.Controls
import QtQuick.Layouts


Rectangle {
    color: "white"
    anchors.fill: parent
    // Layout.fillWidth: true
    // Layout.fillHeight: true

    ColumnLayout {
        anchors.fill: parent
        // Layout.fillWidth: true
        // Layout.fillHeight: true

        // Thanh tiêu đề
        ProcessHeader {}

        // Danh sách tiến trình
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: ListModel {
                ListElement { name: "Qt Creator"; cpu: "2%"; memory: "503MB"; disk: "0MB/s"; network: "0 Mbps" }
                ListElement { name: "Google Chrome"; cpu: "3%"; memory: "248MB"; disk: "0MB/s"; network: "0 Mbps" }
                ListElement { name: "Microsoft Edge"; cpu: "0%"; memory: "87MB"; disk: "0MB/s"; network: "0 Mbps" }
                ListElement { name: "Qt Creator"; cpu: "2%"; memory: "503MB"; disk: "0MB/s"; network: "0 Mbps" }
                ListElement { name: "Google Chrome"; cpu: "3%"; memory: "248MB"; disk: "0MB/s"; network: "0 Mbps" }
                ListElement { name: "Microsoft Edge"; cpu: "0%"; memory: "87MB"; disk: "0MB/s"; network: "0 Mbps" }
                ListElement { name: "Qt Creator"; cpu: "2%"; memory: "503MB"; disk: "0MB/s"; network: "0 Mbps" }
                ListElement { name: "Google Chrome"; cpu: "3%"; memory: "248MB"; disk: "0MB/s"; network: "0 Mbps" }
                ListElement { name: "Microsoft Edge"; cpu: "0%"; memory: "87MB"; disk: "0MB/s"; network: "0 Mbps" }
            }

            delegate: ProcessRow {
                id: rowDelegate
                required property var model
                name: model.name
                // status: model.status
                cpu: model.cpu
                memory: model.memory
                disk: model.disk
                network: model.network
            }
        }
    }
}
