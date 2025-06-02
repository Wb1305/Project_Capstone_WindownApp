import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import "../Performance"
import "../StressTest"

RowLayout {
    id: mainPerformance
    spacing: 5

    property var performModel
    property var preferHeight
    property var preferWidth
    property var parentHeight

    ColumnLayout{
        // Button để kích hoạt khu vực Stress Test

        spacing: 5

        Button {
            id: btnStess
            text: stressTest.isToggle ? "CLOSE STRESS" : "OPEN STRESS"
            font.pixelSize: 14
            font.bold: true

            Layout.preferredWidth: 130
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignTop

            onClicked: {
                stressTest.isToggle = !stressTest.isToggle
            }
        }

        StressTestView{
            id: stressTest
            property bool isToggle: false
            isToggleChanged: isToggle

            Layout.preferredWidth: isToggle ? 250 : btnStess.width
            // Layout.preferredHeight: mainPerformance.parentHeight - btnStess.height - 10
            Layout.fillHeight: true

            color: isToggle ? "gray" : "transparent"
            // border.color: "blue"

            Behavior on Layout.preferredWidth {
                NumberAnimation { duration: 500; easing.type: Easing.InOutQuad }
            }

            Behavior on color {
                ColorAnimation {
                    duration: 500  // Thời gian chuyển màu
                    easing.type: Easing.InOutQuad  // Easing để tạo hiệu ứng mượt mà
                }
            }
        }
    }

    PerformanceView{
        // Layout.preferredWidth: mainPerformance.preferWidth - stressTest.width
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.preferredWidth: mainPerformance.preferWidth - (stressTest.isToggle ? stressTest.width : btnStess.width)
        Layout.preferredHeight: mainPerformance.preferHeight

        performModel: mainPerformance.performModel
        preferHeight: mainPerformance.preferHeight
        preferWidth: mainPerformance.preferWidth - (stressTest.isToggle ? stressTest.width : btnStess.width)
    }
}
