
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

pragma ComponentBehavior: Bound


Rectangle {
    id: procTableView
    anchors.fill: parent
    anchors.leftMargin: 30
    required property var procModel
    required property var setHeight
    property int hoveredRow: -1
    // color: "red"

    ColumnUtils {
        id: columnUtils
        columnCount: procTableView.procModel.columnCount
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
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            clip: true
            TableView {
                id: tableViewData
                implicitWidth: columnUtils.getTotalColumnWidth()
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


