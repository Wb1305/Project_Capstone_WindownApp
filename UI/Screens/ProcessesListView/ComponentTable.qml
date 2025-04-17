import QtQuick

Rectangle {
    id: rectComp
    implicitHeight: 40

    required property int row
    required property int column
    required property var model
    required property var tableViewData

    property bool isHovered: row === tableViewData.hoveredRow
    property var value: model.getData(row, model.roleNameAt(column))
    property string roleName: model.roleNameAt(column)
    property var maxRam: model.maxRam

    ColorUtils{
        id: colorUtils
    }

    color: {
        if (isHovered)
            return "#d0e8ff";

        // Tô màu theo role
        if (roleName === "cpu" || roleName === "mem" || roleName === "memMB") {
            let maxValue = roleName === "memMB" ? rectComp.maxRam : 100;
            let ratio = Math.min(Number(value) / maxValue, 1.0);
            let base = Qt.rgba(1, 1, 0.7, 1);         // vàng nhạt
            let highlight = Qt.rgba(1, 0.8, 0, 1);    // vàng đậm
            return colorUtils.lerpColor(base, highlight, ratio)
        }

        return "#ffffff";
    }

    Text {
        id: textcontent
        text: {
            const role = rectComp.roleName
            const val = rectComp.value

            if (val === undefined || val === null)
                return "N/A"

            if (role === "cpu" || role === "mem")
                return Number(val).toFixed(2) + " %"

            if (role === "memMB")
                return Number(val).toFixed(2) + " MB"
                // return Number(val).toLocaleString(undefined, { minimumFractionDigits: 2, maximumFractionDigits: 2 })

            return val.toString()
        }

        font.pixelSize: 14
        color: "#333"
        elide: Text.ElideRight
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.left: (rectComp.roleName === "name") ? parent.left : undefined
        anchors.right: (rectComp.roleName === "cpu" || rectComp.roleName === "mem" || rectComp.roleName === "memMB") ? parent.right : undefined
        anchors.centerIn: (rectComp.roleName === "user" || rectComp.roleName === "pid") ? parent : undefined
        // Căn lề thông minh
        horizontalAlignment: {
            if (rectComp.roleName === "cpu" || rectComp.roleName === "mem" || rectComp.roleName === "memMB")
                return Text.AlignRight
            if (rectComp.roleName === "name")
                return Text.AlignLeft
            return Text.AlignHCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: rectComp.tableViewData.hoveredRow = rectComp.row
        onExited: {
            // chỉ clear nếu đang hover đúng row này
            if (rectComp.tableViewData.hoveredRow === rectComp.row)
                rectComp.tableViewData.hoveredRow = -1
        }
    }

    // vẻ đường viền
    Rectangle {
        width: 1
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        color: "#bbb"
    }
}

