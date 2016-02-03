import QtQuick 2.0

Rectangle {
    id: calibration
    color: "white";
    property real interpipillaryDistance: 0;
    property real density: 50;
    property color gridColor: "black";

    focus: true;

    function _renderGrid(context)
    {
        for (var x = 0.5; x < width; x += calibration.density) {
            context.moveTo(x, 0);
            context.lineTo(x, width);
        }

        for (var y = 0.5; y < height; y += calibration.density) {
            context.moveTo(0, y);
            context.lineTo(height, y);
        }

        context.strokeStyle = gridColor;
        context.stroke();
    }

    Canvas{
        id: grid;
     /*   Text{
            text: "c2:"+Math.round(calibration.coefficient2*100)/100;
            anchors.centerIn: parent;
            font.pointSize: 20;
        }*/
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        anchors.leftMargin: calibration.interpipillaryDistance/2.0;
        width: parent.width;

        onPaint: {
            var context = getContext("2d");
            calibration._renderGrid(context);
        }
    }
}
