
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

pragma ComponentBehavior: Bound


Rectangle {
    id: procTableView
    anchors.fill: parent
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    required property var procModel
    required property var setHeight
    required property var setWidth

    property int hoveredRow: -1
    // color: "red"

    ColumnUtils {
        id: columnUtils
        columnCount: procTableView.procModel.columnCount
        maxWidth: procTableView.setWidth
    }

    ColumnLayout {
        id: columnLayout
        // anchors.fill: parent
        Layout.fillHeight: true
        Layout.fillWidth: true
        spacing: 0

        // Header ngoài TableView
        HeaderView {
            id: headerView
            model: procTableView.procModel
            columnUtils: columnUtils
        }

        ScrollView{
            id: scrollItem
            Layout.fillWidth: true
            Layout.preferredHeight: tableViewData.implicitHeight
            // Ẩn scrollbar
            ScrollBar.horizontal.policy: ScrollBar.AsNeeded
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            // ScrollBar.vertical.policy: ScrollBar.AlwaysOff
            // ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            clip: true
            TableView {
                id: tableViewData
                width: columnUtils.getTotalColumnWidth()
                implicitHeight: procTableView.setHeight - headerView.implicitHeight
                model: procTableView.procModel

                // Sử dụng chung hàm columnWidthProvider
                columnWidthProvider: function(column) {
                    return columnUtils.getColumnWidth(column);
                }
                delegate: ComponentTable
                {
                    model: tableViewData.model
                    tableViewData: procTableView
                }

            }
        }
    }
}


