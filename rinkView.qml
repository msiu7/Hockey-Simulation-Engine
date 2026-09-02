import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 880
    height: 420

    // Coordinate scaling from 220x105 sim space
    readonly property real scaleFactor: width / 220.0

    //Ice Rink Background
    Canvas {
        id: rinkCanvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            ctx.scale(root.scaleFactor, root.scaleFactor);

            // Base ice (0,0 to 200,85)
            ctx.fillStyle = "#f5faff";
            ctx.beginPath();
            ctx.roundedRect(0, 0, 200, 85, 28, 28);
            ctx.fill();

            // Center Red Line
            ctx.strokeStyle = "#d72638";
            ctx.lineWidth = 1.0;
            ctx.beginPath();
            ctx.moveTo(100, 0);
            ctx.lineTo(100, 85);
            ctx.stroke();

            // Blue Lines
            ctx.strokeStyle = "#1e64c8";
            ctx.beginPath();
            ctx.moveTo(75, 0); ctx.lineTo(75, 85);
            ctx.moveTo(125, 0); ctx.lineTo(125, 85);
            ctx.stroke();
        }
    }

    //Scoreboard Overlay
    Rectangle {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 8
        width: 160
        height: 32
        radius: 6
        color: Qt.rgba(0, 0, 0, 0.65)

        Text {
            anchors.centerIn: parent
            color: "white"
            font.bold: true
            text: (simEngine ? "P" + simEngine.period + " | " + simEngine.timerText : "00:00")
        }
    }

    // Away Players (Red dots)
    Repeater {
        model: simEngine ? simEngine.awayPlayers : []
        delegate: Rectangle {
            width: 14
            height: 14
            radius: 7
            color: "#d72638"
            border.color: "black"
            border.width: 1

            // Position scaled to the rink width (200 ft standard width)
            x: (modelData.x / 200.0) * root.width - (width / 2)
            y: (modelData.y / 85.0) * root.height - (height / 2)

            Text {
                anchors.centerIn: parent
                text: modelData.number
                color: "white"
                font.pixelSize: 8
                font.bold: true
            }
        }
    }

    // Home Players (Blue dots)
    Repeater {
        model: simEngine ? simEngine.homePlayers : []
        delegate: Rectangle {
            width: 14
            height: 14
            radius: 7
            color: "#1e64c8"
            border.color: "black"
            border.width: 1

            x: (modelData.x / 200.0) * root.width - (width / 2)
            y: (modelData.y / 85.0) * root.height - (height / 2)

            Text {
                anchors.centerIn: parent
                text: modelData.number
                color: "white"
                font.pixelSize: 8
                font.bold: true
            }
        }
    }
}