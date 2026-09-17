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

        function drawGoalCrease(c, goalLineX, centerY, facingRight) {
            c.save();
            c.translate(goalLineX, centerY);
            // When facing left (facingRight === false), mirror the X axis so the exact same coordinates work
            if (!facingRight) {
                c.scale(-1, 1);
            }

            const halfW = 4.0;         // 8 ft total width (4 ft above / below center)
            const straightX = 4.0;     // 4 ft straight wall length
            const radius = 6.0;        // 6 ft radius from goal line center
            // Angle where circle of radius 6 meets y = 4: sin(theta) = 4 / 6
            const theta = Math.asin(halfW / radius);

            c.fillStyle = creaseColor;
            c.strokeStyle = redLine;
            c.lineWidth = 0.6;

            c.beginPath();
            // 1. Base on goal line (top)
            c.moveTo(0, -halfW);
            // 2. Straight edge extending 4 ft forward
            c.lineTo(straightX, -halfW);
            // 3. Circular front cap curving to x = 6 at the apex, then down to (4, +4)
            c.arc(0, 0, radius, -theta, theta, false);
            // 4. Straight edge returning back to goal line (bottom)
            c.lineTo(0, halfW);
            c.closePath();

            c.fill();
            c.stroke();

            // 5. Hash marks (5 inches = ~0.42 ft long, 2 inches wide) located 4 ft out
            const hashLen = 5.0 / 12.0; // ~0.42 ft
            c.lineWidth = 2.0 / 12.0;   // 2 inches wide
            c.beginPath();
            // Top hash mark pointing inward
            c.moveTo(straightX, -halfW);
            c.lineTo(straightX, -halfW + hashLen);
            // Bottom hash mark pointing inward
            c.moveTo(straightX, halfW);
            c.lineTo(straightX, halfW - hashLen);
            c.stroke();

            c.restore();
        }

        function drawGoalCage(c, goalLineX, centerY, facingRight) {
            c.save();
            c.translate(goalLineX, centerY);
            if (!facingRight) {
                c.scale(-1, 1);
            }

            const postDist = 6.0;           // 6 ft across goal line (Y = -3 to +3)
            const halfW = postDist / 2.0;
            const depth = 3.33;             // 4 ft cage depth behind goal line
            const maxBulgeW = 3.6;          // Flares wider than the posts (Limaçon bulge)
            const bookFlatHalfW = 2.0;      // Flat rear back rail width
            const postRadius = 0.4;

            // 1. Skirt / Base Netting (Ice Level Outer Frame)
            c.beginPath();
            c.moveTo(0, -halfW);
            // CP1 flares outward from the post (-0.8, -maxBulgeW)
            // CP2 sits at x = -depth to ensure a clean 90-degree tangent entering the flat back line
            c.bezierCurveTo(-0.8, -maxBulgeW, -depth, -maxBulgeW, -depth, -bookFlatHalfW);
            // Flat vertical back rail
            c.lineTo(-depth, bookFlatHalfW);
            // Bottom curve mirroring back to the bottom post
            c.bezierCurveTo(-depth, maxBulgeW, -0.8, maxBulgeW, 0, halfW);
            c.closePath();

            c.fillStyle = "rgba(230, 235, 245, 0.45)";
            c.fill();
            c.strokeStyle = "#8c98a0";
            c.lineWidth = 0.55;
            c.stroke();

            // 2. Upper Shelf (Inner Limaçon Arc)
            const shelfDepth = 1.8;
            const shelfBulgeW = 3.1;
            const shelfBookFlatHalfW = 1.6;

            c.beginPath();
            c.moveTo(0, -halfW);
            c.bezierCurveTo(-0.5, -shelfBulgeW, -shelfDepth, -shelfBulgeW, -shelfDepth, -shelfBookFlatHalfW);
            c.lineTo(-shelfDepth, shelfBookFlatHalfW);
            c.bezierCurveTo(-shelfDepth, shelfBulgeW, -0.5, shelfBulgeW, 0, halfW);
            c.closePath();

            c.fillStyle = "rgba(245, 248, 255, 0.75)";
            c.fill();
            c.strokeStyle = "#8c98a0";
            c.lineWidth = 0.45;
            c.stroke();

            // 3. Center Spine Pipe
            c.beginPath();
            c.moveTo(0, 0);
            c.lineTo(-depth, 0);
            c.strokeStyle = "#8c98a0";
            c.lineWidth = 0.55;
            c.stroke();

            // 4. Red Crossbar (flush along goal line)
            c.strokeStyle = "#b01207"; //using a different red to be a bit more visible.
            c.lineWidth = 0.7;
            c.lineCap = "round";
            c.beginPath();
            c.moveTo(0, -halfW);
            c.lineTo(0, halfW);
            c.stroke();

            // 5. Goal Posts
            c.fillStyle = "#b01207";
            c.beginPath();
            c.arc(0, -halfW, postRadius, 0, 2 * Math.PI);
            c.arc(0, halfW, postRadius, 0, 2 * Math.PI);
            c.fill();

            c.restore();
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

            // Call on the ice surface:
            drawGoalCrease(ctx, goalLineLeftX, centerY, true);
            drawGoalCrease(ctx, goalLineRightX, centerY, false);

            // Goal Cages
            drawGoalCage(ctx, goalLineLeftX, centerY, true);
            drawGoalCage(ctx, goalLineRightX, centerY, false);
        }
    }
    //away players
    Repeater {
        model: simEngine ? simEngine.awayPlayers : []
        delegate: Item { // Outer container holds rink position
            width: (2.0 / 220.0) * root.width
            height: (2.0 / 105.0) * root.height

            x: ((modelData.x + 10.0) / 220.0) * root.width - (width / 2)
            y: ((modelData.y + 10.0) / 105.0) * root.height - (height / 2)

            // Rotate the visual components around their center
            rotation: modelData.rotation

            // Player circular body
            Rectangle {
                anchors.fill: parent
                radius: width / 2
                color: "#e03e1d"
                border.color: "black"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: modelData.number
                    color: "white"
                    font.pixelSize: 8
                    font.bold: true
                    // Keep the jersey number right-side up regardless of player spin:
                    rotation: -parent.parent.rotation
                }
            }

            // Direction indicator ("visor" / front pointer)
            Rectangle {
                width: parent.width * 0.35
                height: parent.height * 0.35
                radius: width / 2
                color: "#ffffff"
                border.color: "black"
                border.width: 0.5

                // Positioned at the front edge (right side at angle 0)
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: -width * 0.2
            }
        }
    }
    //home players
    Repeater {
        model: simEngine ? simEngine.homePlayers : []
        delegate: Item { // Outer container holds rink position
            width: (2.0 / 220.0) * root.width
            height: (2.0 / 105.0) * root.height

            x: ((modelData.x + 10.0) / 220.0) * root.width - (width / 2)
            y: ((modelData.y + 10.0) / 105.0) * root.height - (height / 2)

            // Rotate the visual components around their center
            rotation: modelData.rotation

            // Player circular body
            Rectangle {
                anchors.fill: parent
                radius: width / 2
                color: "#1e64c8"
                border.color: "black"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: modelData.number
                    color: "white"
                    font.pixelSize: 8
                    font.bold: true
                    // Keep the jersey number right-side up regardless of player spin:
                    rotation: -parent.parent.rotation
                }
            }

            // Direction indicator ("visor" / front pointer)
            Rectangle {
                width: parent.width * 0.35
                height: parent.height * 0.35
                radius: width / 2
                color: "#ffffff"
                border.color: "black"
                border.width: 0.5

                // Positioned at the front edge (right side at angle 0)
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: -width * 0.2
            }
        }
    }

    Repeater {
        model: (simEngine && simEngine.puck) ? 1 : 0
        delegate: Rectangle {
            width: 8
            height: 8
            radius: 4
            color: "black"
            border.color: "white"
            border.width: 1

                x: (((simEngine.puck.x + 10.0) / 220.0) * root.width) - (width / 2)
                y: (((simEngine.puck.y + 10.0) / 105.0) * root.height) - (height / 2)
            }
        }
}