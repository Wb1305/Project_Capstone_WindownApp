import QtQuick
import QtQuick.Controls

Dialog {
    id: overloadDialog

    property var systemMonitorModel
    property var parentWidth
    property var parentHeight
    property string message: "Đóng ứng dụng không cần thiết để giảm tải hệ thống!"

    title: "⚠️ Cảnh báo hệ thống quá tải!"
    font.pixelSize: 15
    standardButtons: Dialog.Ok | Dialog.Cancel
    visible: false
    modal: true
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 500
    height: 150

    padding: 0

    background:
        Rectangle {
        color: "lightblue"
        border.color: "red"
        border.width: 3
        radius: 10
    }

    //--- content ---
    contentItem: Label {
        text: overloadDialog.message
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 15
        color: "black"
        anchors.fill: parent
        anchors.margins: 16
    }

    onAccepted: console.log("Ok clicked")
    onRejected: console.log("Cancel clicked")

    Connections{
        target: overloadDialog.systemMonitorModel
        function onInformOverloadInfoForUI(info){
            overloadDialog.visible = true
            var shutdownTime = new Date(info.shutdownTime)
            var dateStr = Qt.formatDateTime(shutdownTime, "dd/MM/yyyy")
            var timeStr = Qt.formatDateTime(shutdownTime, "hh:mm:ss")
            var message = dateStr + " - Vào lúc " + timeStr + ": Đã đóng ứng dụng \"" + info.procName + "\""

            overloadDialog.message = message
        }
    }

    //--- hiệu ứng ---
    opacity: 0.0
    scale: 0.9
    transformOrigin: Item.Center

    onVisibleChanged: {
        if (visible) {
            fadeInAnim.restart()
        }
    }

    SequentialAnimation {
        id: fadeInAnim
        running: false
        PropertyAnimation { target: overloadDialog; property: "opacity"; to: 1.0; duration: 300; easing.type: Easing.InOutQuad }
        PropertyAnimation { target: overloadDialog; property: "scale"; to: 1.0; duration: 300; easing.type: Easing.OutBack }
    }
}
