import QtQuick

QtObject {
    id: colorUtils

    // Hàm nội suy màu: t từ 0.0 đến 1.0
    function lerpColor(color1, color2, t) {
        return Qt.rgba(
            color1.r + (color2.r - color1.r) * t,
            color1.g + (color2.g - color1.g) * t,
            color1.b + (color2.b - color1.b) * t,
            color1.a + (color2.a - color1.a) * t
        );
    }
}
