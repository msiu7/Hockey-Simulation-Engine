import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 880
    height: 420

    // Coordinate scaling from 220x105 sim space
    readonly property real scaleFactor: width / 220.0

    Canvas {
        id: rinkCanvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            // scale
            var scaleX = width / 220.0;
            var scaleY = height / 105.0;
            ctx.scale(scaleX, scaleY);
            // Shift Y down by 10 units so (0, 0) to (200, 85) is the ice surface
            ctx.translate(10.0, 10.0);

            const rinkW = 200.0;
            const rinkH = 85.0;
            const centerY = rinkH / 2.0;
            const cornerRadius = 28.0;

            //colors
            const iceColor = "#f5faff";
            const redLine = "#d72638";
            const blueLine = "#1e64c8";
            const creaseColor = "#b4e1fa";
            const boardDark = "#282828";
            const boardGold = "#ebc32d";


            function roundedRect(c, x, y, w, h, r) {
                c.beginPath();
                c.moveTo(x + r, y);
                c.arcTo(x + w, y,     x + w, y + h, r); // Top-right corner
                c.arcTo(x + w, y + h, x,     y + h, r); // Bottom-right corner
                c.arcTo(x,     y + h, x,     y,     r); // Bottom-left corner
                c.arcTo(x,     y,     x + w, y,     r); // Top-left corner
                c.closePath();
            }

            function drawFaceoffCircle(c, cx, cy) {
                const radius = 15.0;
                // Circle
                c.strokeStyle = redLine;
                c.lineWidth = 0.6;
                c.beginPath();
                c.arc(cx, cy, radius, 0, 2 * Math.PI);
                c.stroke();

                // Center Dot
                c.fillStyle = redLine;
                c.beginPath();
                c.arc(cx, cy, 1.0, 0, 2 * Math.PI);
                c.fill();

                // Hashmarks (top and bottom)
                const hashSpacing = 2.85;
                const hashLen = 2.0;
                c.beginPath();
                c.moveTo(cx - hashSpacing, cy - radius); c.lineTo(cx - hashSpacing, cy - radius - hashLen);
                c.moveTo(cx + hashSpacing, cy - radius); c.lineTo(cx + hashSpacing, cy - radius - hashLen);
                c.moveTo(cx - hashSpacing, cy + radius); c.lineTo(cx - hashSpacing, cy + radius + hashLen);
                c.moveTo(cx + hashSpacing, cy + radius); c.lineTo(cx + hashSpacing, cy + radius + hashLen);
                c.stroke();
            }

            function drawGoalCage(c, goalLineX, centerY, facingRight) {
                const goalWidth = 6.0;   // 6 ft between posts (Y-axis)[cite: 3]
                const goalDepth = 4.0;   // 4 ft deep to center back (X-axis)[cite: 3]
                const halfWidth = goalWidth / 2.0;

                // Top post, bottom post, and back curve coordinates
                const topPostY = centerY - halfWidth;
                const bottomPostY = centerY + halfWidth;

                // Directing whether the cage extends leftward (-X) or rightward (+X)
                const dir = facingRight ? -1.0 : 1.0;
                const backX = goalLineX + (dir * goalDepth);
                const cornerBackX = goalLineX + (dir * (goalDepth - 1.2)); // Where side tapers meet back radius

                //Netting interior path
                c.beginPath();
                c.moveTo(goalLineX, topPostY);
                // Upper side pipe tapering backward
                c.lineTo(cornerBackX, centerY - 2.2);
                // Back curve arc
                c.quadraticCurveTo(backX, centerY, cornerBackX, centerY + 2.2);
                // Lower side pipe returning to bottom post
                c.lineTo(goalLineX, bottomPostY);
                c.closePath();

                // Semi-transparent mesh fill
                c.fillStyle = "rgba(225, 225, 230, 0.55)";
                c.fill();

                //Red Pipe Outlines
                c.strokeStyle = "#d72638";
                c.lineWidth = 0.8;
                c.lineCap = "round";
                c.lineJoin = "round";

                // Main frame: post -> corner -> rounded back -> corner -> post
                c.beginPath();
                c.moveTo(goalLineX, topPostY);
                c.lineTo(cornerBackX, centerY - 2.2);
                c.quadraticCurveTo(backX, centerY, cornerBackX, centerY + 2.2);
                c.lineTo(goalLineX, bottomPostY);
                c.stroke();

                // Center rear support post
                c.lineWidth = 0.4;
                c.beginPath();
                c.moveTo(backX, centerY);
                c.lineTo(goalLineX + (dir * 1.5), centerY);
                c.stroke();

                // 3. Goal Posts on Goal Line
                c.fillStyle = "#d72638";
                c.beginPath();
                c.arc(goalLineX, topPostY, 0.5, 0, 2 * Math.PI);
                c.arc(goalLineX, bottomPostY, 0.5, 0, 2 * Math.PI);
                c.fill();
            }

            //benches and penalty boxes
            ctx.lineWidth = 0.6;
            ctx.strokeStyle = "#3c3c3c";


            ctx.fillStyle = "#f0dcdc"; // Home bench
            ctx.fillRect(62.0, -6.5, 34.0, 6.0);
            ctx.strokeRect(62.0, -6.5, 34.0, 6.0);

            ctx.fillStyle = "#d2e1f0"; // Away bench
            ctx.fillRect(104.0, -6.5, 34.0, 6.0);
            ctx.strokeRect(104.0, -6.5, 34.0, 6.0);

            // Penalty Boxes
            ctx.fillStyle = "#ebebeb";
            ctx.fillRect(74.0, rinkH + 0.5, 14.0, 5.5);
            ctx.strokeRect(74.0, rinkH + 0.5, 14.0, 5.5);
            ctx.fillRect(112.0, rinkH + 0.5, 14.0, 5.5);
            ctx.strokeRect(112.0, rinkH + 0.5, 14.0, 5.5);

            ctx.fillStyle = "#f5f5e6";
            ctx.fillRect(91.0, rinkH + 0.5, 18.0, 5.5);
            ctx.strokeRect(91.0, rinkH + 0.5, 18.0, 5.5);

            //ice and markings
            ctx.save();
            roundedRect(ctx, 0, 0, rinkW, rinkH, cornerRadius);
            ctx.clip(); // Keeps ice lines from leaking outside boards

            // Base Ice
            ctx.fillStyle = iceColor;
            ctx.fill();

            // Goal Lines
            const goalLineLeftX = 11.0; //11 ft from back
            const goalLineRightX = rinkW - 11.0;
            ctx.strokeStyle = redLine;
            ctx.lineWidth = 0.6;
            ctx.beginPath();
            ctx.moveTo(goalLineLeftX, 0); ctx.lineTo(goalLineLeftX, rinkH);
            ctx.moveTo(goalLineRightX, 0); ctx.lineTo(goalLineRightX, rinkH);
            ctx.stroke();

            //creases
            ctx.fillStyle = creaseColor;
            ctx.beginPath();
            ctx.arc(goalLineLeftX, centerY, 6.0, -0.5 * Math.PI, 0.5 * Math.PI, false);
            ctx.fill();
            ctx.stroke();

            ctx.beginPath();
            ctx.arc(goalLineRightX, centerY, 6.0, 0.5 * Math.PI, 1.5 * Math.PI, false);
            ctx.fill();
            ctx.stroke();

            //trapezoids
            ctx.beginPath();
            ctx.moveTo(0, centerY - 14.0); ctx.lineTo(goalLineLeftX, centerY - 11.0);
            ctx.moveTo(0, centerY + 14.0); ctx.lineTo(goalLineLeftX, centerY + 11.0);
            ctx.moveTo(rinkW, centerY - 14.0); ctx.lineTo(goalLineRightX, centerY - 11.0);
            ctx.moveTo(rinkW, centerY + 14.0); ctx.lineTo(goalLineRightX, centerY + 11.0);
            ctx.stroke();

            //blue lines
            ctx.strokeStyle = blueLine;
            ctx.lineWidth = 1.0;
            ctx.beginPath();
            ctx.moveTo(75.0, 0); ctx.lineTo(75.0, rinkH);
            ctx.moveTo(125.0, 0); ctx.lineTo(125.0, rinkH);
            ctx.stroke();

            //center line
            ctx.strokeStyle = redLine;
            ctx.beginPath();
            ctx.moveTo(100.0, 0); ctx.lineTo(100.0, rinkH);
            ctx.stroke();

            //center circle
            ctx.strokeStyle = blueLine;
            ctx.lineWidth = 0.6;
            ctx.beginPath();
            ctx.arc(100.0, centerY, 15.0, 0, 2 * Math.PI);
            ctx.stroke();
            ctx.fillStyle = blueLine;
            ctx.beginPath();
            ctx.arc(100.0, centerY, 1.0, 0, 2 * Math.PI);
            ctx.fill();

            //d zone circles
            drawFaceoffCircle(ctx, goalLineLeftX + 20.0, centerY - 22.0);
            drawFaceoffCircle(ctx, goalLineLeftX + 20.0, centerY + 22.0);
            drawFaceoffCircle(ctx, goalLineRightX - 20.0, centerY - 22.0);
            drawFaceoffCircle(ctx, goalLineRightX - 20.0, centerY + 22.0);

            //neutral zone dots
            ctx.fillStyle = redLine;
            const nzDotsX = [80.0, 120.0];
            const nzDotsY = [centerY - 22.0, centerY + 22.0];
            for (let i = 0; i < nzDotsX.length; ++i) {
                for (let j = 0; j < nzDotsY.length; ++j) {
                    ctx.beginPath();
                    ctx.arc(nzDotsX[i], nzDotsY[j], 1.0, 0, 2 * Math.PI);
                    ctx.fill();
                }
            }

            // Release ice clipping
            ctx.restore();

            // --- 3. Boards & Nets ---
            // Boards Outer Boundary
            ctx.strokeStyle = boardDark;
            ctx.lineWidth = 1.2;
            roundedRect(ctx, 0, 0, rinkW, rinkH, cornerRadius);
            ctx.stroke();

            //gold boards
            ctx.strokeStyle = boardGold;
            ctx.lineWidth = 0.5;
            roundedRect(ctx, 0.4, 0.4, rinkW - 0.8, rinkH - 0.8, cornerRadius - 0.4);
            ctx.stroke();

            // Goal Cages
            drawGoalCage(ctx, goalLineLeftX, centerY, true);
            drawGoalCage(ctx, goalLineRightX, centerY, false);
        }
    }

    // Away Players (Red dots)
    Repeater {
        model: simEngine ? simEngine.awayPlayers : []
        delegate: Rectangle {
            width: (2.0 / 220.0) * root.width
            height: (2.0 / 105.0) * root.height
            radius: width / 2
            color: "#d72638"
            border.color: "black"
            border.width: 1

            // scaling to actual window size
            x: ((modelData.x + 10.0) / 220.0) * root.width - (width / 2)
            y: ((modelData.y + 10.0) / 105.0) * root.height - (height / 2)

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
            width: (2.0 / 220.0) * root.width
            height: (2.0 / 105.0) * root.height
            radius: width / 2
            color: "#1e64c8"
            border.color: "black"
            border.width: 1

            // scaling to actual window size
            x: ((modelData.x + 10.0) / 220.0) * root.width - (width / 2)
            y: ((modelData.y + 10.0) / 105.0) * root.height - (height / 2)

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