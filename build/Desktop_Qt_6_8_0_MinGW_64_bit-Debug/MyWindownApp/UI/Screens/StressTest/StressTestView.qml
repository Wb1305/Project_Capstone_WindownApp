import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../../Components"

Rectangle{
    id: stresstest
    property bool isToggleChanged
    property var preferWidth: 200

    color: "gray"
    topRightRadius: 10
    bottomRightRadius: 10

    ColumnLayout {
        id: inputLayoutStress
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 15

        // visible: stresstest.isToggleChanged ? true : false
        visible: opacity > 0
        opacity: stresstest.isToggleChanged ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 500 }
        }

        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20

        // Nhập số task cần chạy
        RowLayout {
            spacing: 10
            Layout.preferredWidth: stresstest.preferWidth

            CustomLabel{
                text: "Number of Tasks:"
                Layout.alignment: Qt.AlignVCenter
            }
            SpinBox {
                id: numberOfTaskSpin
                from: 1
                to: 10
                value: 1
                Layout.preferredWidth: 50
                font.pixelSize: 14
                editable: true
                onValueChanged: {
                    if (numberOfTaskSpin.value < numberOfTaskSpin.from) {
                        numberOfTaskSpin.value = numberOfTaskSpin.from;
                    } else if (numberOfTaskSpin.value > numberOfTaskSpin.to) {
                        numberOfTaskSpin.value = numberOfTaskSpin.to;
                    }
                }
            }
        }

        // Nhập phần trăm MEM Usage
        ColumnLayout {
            Layout.preferredWidth: stresstest.preferWidth
            spacing: 10
            CustomLabel {
                text: "MEM Usage Percent:"
                Layout.alignment: Qt.AlignVCenter
            }

            RowLayout{
                spacing: 5
                Slider {
                    id: memUsageSlider
                    from: 0
                    to: 100
                    value: 0
                    stepSize: 1
                    Layout.preferredWidth: 150
                }
                Text {
                    text: Math.round(memUsageSlider.value) + "%"
                    color: "white"
                    Layout.alignment: Qt.AlignCenter
                }
            }

        }

        // Nhập số lượng core
        RowLayout {
            Layout.preferredWidth: stresstest.preferWidth
            spacing: 10
            CustomLabel {
                text: "Number of Cores:"
                Layout.alignment: Qt.AlignVCenter
            }
            SpinBox {
                id: numberOfCoreSpin
                from: 1
                to: 8
                value: 8
                Layout.preferredWidth: 50
                font.pixelSize: 14
                editable: true
                onValueChanged: {
                    if (numberOfTaskSpin.value < numberOfTaskSpin.from) {
                        numberOfTaskSpin.value = numberOfTaskSpin.from;
                    } else if (numberOfTaskSpin.value > numberOfTaskSpin.to) {
                        numberOfTaskSpin.value = numberOfTaskSpin.to;
                    }
                }
            }
        }

        // Nhập timeout
        RowLayout {
            Layout.preferredWidth: stresstest.preferWidth
            spacing: 10
            CustomLabel {
                text: "Timeout (seconds):"
                Layout.alignment: Qt.AlignVCenter
            }

            Rectangle {
                Layout.preferredWidth: 70
                Layout.preferredHeight: 30
                color: "white"
                // radius: 5  // Bo góc
                border.color: "gray"
                border.width: 1
                clip: true

                TextInput {
                    id: timeoutInput
                    text: "0"
                    font.pixelSize: 14
                    color: "black"
                    anchors.fill: parent
                    // leftPadding: 5
                    // rightPadding: 5
                    padding: 5
                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    verticalAlignment: TextInput.AlignVCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    HoverHandler {
                        cursorShape: Qt.PointingHandCursor
                    }
                }
            }
        }

        // Button để xác nhận các tham số

        RowLayout{
            Layout.preferredWidth: stresstest.preferWidth
            Layout.alignment: Qt.AlignCenter

            CustomButton{
                buttonName: "START"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        console.log("Start stress test");

                        // console.log("Number of Tasks:", numberOfTaskSpin.value);
                        // console.log("MEM Usage Percent:", Math.round(memUsageSlider.value));
                        // console.log("Number of Cores:", numberOfCoreSpin.value);
                        // console.log("Timeout:", timeoutInput.text);

                        let numTasks = parseInt(numberOfTaskSpin.value);
                        let memUsage = Math.round(memUsageSlider.value);
                        let numCores = parseInt(numberOfCoreSpin.value);
                        let timeOut = parseInt(timeoutInput.text);

                        SystemMonitor.startStressTest(numTasks, memUsage, numCores, timeOut); // đã test ok

                    }
                }
            }

            CustomButton{
                buttonName: "STOP"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        console.log("Stop stress test");

                        SystemMonitor.stopStressTest();
                    }
                }
            }
        }
    }
}
