import QtQuick

QtObject {
    function getColumnWidth(column) {
        switch(column) {
        case 0: return 350;
        case 1: return 100;
        case 2: return 100;
        case 3: return 100;
        case 4: return 100;
        case 5: return 100;
        default: return 100;
        }
    }

    function getTotalColumnWidth() {
        let total = 0;
        for (let i = 0; i <= 5; i++) {
            total += getColumnWidth(i);
        }
        return total;
    }
}
