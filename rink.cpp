#include "rink.hpp"
#include <QtMath>

rink::rink(QObject *parent) : QGraphicsScene(parent) {
    //Scene Bounds: 200x85, plus 10 in both y directions for benches and penalty boxes.
    //the rink itself is from (0, 0) to (200, 85)
    setSceneRect(0.0, -10.0, 220.0, 105.0);
}

void rink::drawBackground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);
    painter->setRenderHint(QPainter::Antialiasing, true);

    const qreal rinkW = 200.0;
    const qreal rinkH = 85.0;
    const qreal centerY = rinkH / 2.0;
    const qreal cornerRadius = 28.0;

    //colors
    const QColor iceColor(245, 250, 255);
    const QColor redLine(215, 38, 56);
    const QColor blueLine(30, 100, 200);
    const QColor goalCrease(180, 225, 250);
    const QColor boardsColor(230, 230, 230);
    const QColor glassColor(180, 210, 230, 70);

    //benches and boxes
    drawBenchesAndBoxes(painter, rinkW, rinkH);

    //ice surface
    QPainterPath rinkPath;
    rinkPath.addRoundedRect(QRectF(0, 0, rinkW, rinkH), cornerRadius, cornerRadius);

    painter->save();
    //clipping: get rid of lines bleeding outside the boards
    painter->setClipPath(rinkPath);

    //base ice
    painter->setPen(Qt::NoPen);
    painter->setBrush(iceColor);
    painter->drawPath(rinkPath);

    //goal lines (11 ft from back boards)
    qreal goalLineLeftX = 11.0;
    qreal goalLineRightX = rinkW - 11.0;

    painter->setPen(QPen(redLine, 0.6));
    painter->drawLine(QPointF(goalLineLeftX, 0), QPointF(goalLineLeftX, rinkH));
    painter->drawLine(QPointF(goalLineRightX, 0), QPointF(goalLineRightX, rinkH));

    //creases (6ft radius semicircle)
    painter->setBrush(goalCrease);
    painter->setPen(QPen(redLine, 0.6));
    painter->drawPie(QRectF(goalLineLeftX - 6.0, centerY - 6.0, 12.0, 12.0), -90 * 16, 180 * 16);
    painter->drawPie(QRectF(goalLineRightX - 6.0, centerY - 6.0, 12.0, 12.0), 90 * 16, 180 * 16);

    //trapezoids
    painter->setPen(QPen(redLine, 0.6));
    painter->drawLine(QPointF(0, centerY - 14.0), QPointF(goalLineLeftX, centerY - 11.0));
    painter->drawLine(QPointF(0, centerY + 14.0), QPointF(goalLineLeftX, centerY + 11.0));
    painter->drawLine(QPointF(rinkW, centerY - 14.0), QPointF(goalLineRightX, centerY - 11.0));
    painter->drawLine(QPointF(rinkW, centerY + 14.0), QPointF(goalLineRightX, centerY + 11.0));

    //blue lines (75 ft from end boards -> 12 inches / 1.0 ft thick)
    painter->setPen(QPen(blueLine, 1.0));
    painter->drawLine(QPointF(75.0, 0), QPointF(75.0, rinkH));
    painter->drawLine(QPointF(125.0, 0), QPointF(125.0, rinkH));

    //center red line (100 ft -> 1.0 ft thick)
    painter->setPen(QPen(redLine, 1.0));
    painter->drawLine(QPointF(100.0, 0), QPointF(100.0, rinkH));

    //center faceoff circle
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(blueLine, 0.6));
    painter->drawEllipse(QPointF(100.0, centerY), 15.0, 15.0);
    painter->setBrush(blueLine);
    painter->drawEllipse(QPointF(100.0, centerY), 1.0, 1.0); // Center dot

    //defensive zone faceoff circles
    qreal endZoneX_Left = goalLineLeftX + 20.0;
    qreal endZoneX_Right = goalLineRightX - 20.0;
    qreal endZoneY_Top = centerY - 22.0;
    qreal endZoneY_Bottom = centerY + 22.0;

    drawFaceoffCircleWithHashmarks(painter, endZoneX_Left, endZoneY_Top, redLine);
    drawFaceoffCircleWithHashmarks(painter, endZoneX_Left, endZoneY_Bottom, redLine);
    drawFaceoffCircleWithHashmarks(painter, endZoneX_Right, endZoneY_Top, redLine);
    drawFaceoffCircleWithHashmarks(painter, endZoneX_Right, endZoneY_Bottom, redLine);

    // neutral zone faceoff dots (2 ft diameter dots)
    painter->setPen(Qt::NoPen);
    painter->setBrush(redLine);
    painter->drawEllipse(QPointF(80.0, endZoneY_Top), 1.0, 1.0);
    painter->drawEllipse(QPointF(80.0, endZoneY_Bottom), 1.0, 1.0);
    painter->drawEllipse(QPointF(120.0, endZoneY_Top), 1.0, 1.0);
    painter->drawEllipse(QPointF(120.0, endZoneY_Bottom), 1.0, 1.0);

    painter->restore(); // release clipping

    //boards
    painter->setPen(QPen(QColor(40, 40, 40), 1.2));
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(QRectF(0, 0, rinkW, rinkH), cornerRadius, cornerRadius);

    //gold bottom part of the boards
    painter->setPen(QPen(QColor(235, 195, 45), 0.5));
    painter->drawRoundedRect(QRectF(0.4, 0.4, rinkW - 0.8, rinkH - 0.8), cornerRadius - 0.4, cornerRadius - 0.4);

    //goal cages
    drawGoalCage(painter, goalLineLeftX, centerY, true);   // Left Net (extends leftward to x=7)
    drawGoalCage(painter, goalLineRightX, centerY, false); // Right Net (extends rightward to x=193)
}

void rink::drawFaceoffCircleWithHashmarks(QPainter *painter, qreal cx, qreal cy, const QColor &color) {
    const qreal radius = 15.0;

    //circle and center dot
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(color, 0.6));
    painter->drawEllipse(QPointF(cx, cy), radius, radius);

    painter->setBrush(color);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(cx, cy), 1.0, 1.0);

    //outer hash marks (2 ft long, 5.5 ft apart on top & bottom perimeter)
    painter->setPen(QPen(color, 0.6));
    qreal hashSpacing = 2.85; // Distance from circle center along X axis
    qreal hashLen = 2.0;

    //top perimeter hash marks
    painter->drawLine(QPointF(cx - hashSpacing, cy - radius), QPointF(cx - hashSpacing, cy - radius - hashLen));
    painter->drawLine(QPointF(cx + hashSpacing, cy - radius), QPointF(cx + hashSpacing, cy - radius - hashLen));

    //bottom perimeter hash marks
    painter->drawLine(QPointF(cx - hashSpacing, cy + radius), QPointF(cx - hashSpacing, cy + radius + hashLen));
    painter->drawLine(QPointF(cx + hashSpacing, cy + radius), QPointF(cx + hashSpacing, cy + radius + hashLen));

    //L marks around the faceoff dot
    qreal alignOffset = 2.0; // 2 ft from dot
    qreal lSize = 1.2;

    // Top-Left L
    painter->drawLine(QPointF(cx - alignOffset, cy - alignOffset), QPointF(cx - alignOffset + lSize, cy - alignOffset));
    painter->drawLine(QPointF(cx - alignOffset, cy - alignOffset), QPointF(cx - alignOffset, cy - alignOffset + lSize));

    // Top-Right L
    painter->drawLine(QPointF(cx + alignOffset, cy - alignOffset), QPointF(cx + alignOffset - lSize, cy - alignOffset));
    painter->drawLine(QPointF(cx + alignOffset, cy - alignOffset), QPointF(cx + alignOffset, cy - alignOffset + lSize));

    // Bottom-Left L
    painter->drawLine(QPointF(cx - alignOffset, cy + alignOffset), QPointF(cx - alignOffset + lSize, cy + alignOffset));
    painter->drawLine(QPointF(cx - alignOffset, cy + alignOffset), QPointF(cx - alignOffset, cy - alignOffset - lSize));

    // Bottom-Right L
    painter->drawLine(QPointF(cx + alignOffset, cy + alignOffset), QPointF(cx + alignOffset - lSize, cy + alignOffset));
    painter->drawLine(QPointF(cx + alignOffset, cy + alignOffset), QPointF(cx + alignOffset, cy - alignOffset - lSize));
}

void rink::drawGoalCage(QPainter *painter, qreal goalLineX, qreal centerY, bool facingRight) {
    //NHL Goal: 6 ft wide (Y-axis), 4 ft deep (X-axis)
    const qreal goalWidth = 6.0;
    const qreal goalDepth = 4.0;
    qreal backX = facingRight ? (goalLineX - goalDepth) : (goalLineX + goalDepth);

    //netting
    QRectF netRect;
    if (facingRight) {
        netRect = QRectF(backX, centerY - (goalWidth / 2.0), goalDepth, goalWidth);
    } else {
        netRect = QRectF(goalLineX, centerY - (goalWidth / 2.0), goalDepth, goalWidth);
    }

    //mesh fill
    painter->setPen(QPen(QColor(180, 180, 180, 180), 0.3, Qt::DotLine));
    painter->setBrush(QColor(230, 230, 230, 150));
    painter->drawRoundedRect(netRect, 1.2, 1.2);

    //posts & crossbar
    painter->setPen(QPen(QColor(215, 38, 56), 0.8));
    painter->setBrush(Qt::NoBrush);

    //posts on goal line
    painter->drawPoint(QPointF(goalLineX, centerY - (goalWidth / 2.0)));
    painter->drawPoint(QPointF(goalLineX, centerY + (goalWidth / 2.0)));

    //outer cage pipe
    QPainterPath postPath;
    postPath.moveTo(goalLineX, centerY - (goalWidth / 2.0));
    postPath.lineTo(backX, centerY - (goalWidth / 2.0));
    postPath.lineTo(backX, centerY + (goalWidth / 2.0));
    postPath.lineTo(goalLineX, centerY + (goalWidth / 2.0));
    painter->drawPath(postPath);
}

void rink::drawBenchesAndBoxes(QPainter *painter, qreal rinkW, qreal rinkH) {
    painter->setRenderHint(QPainter::Antialiasing, true);

    QFont labelFont("Arial", 2, QFont::Bold);
    painter->setFont(labelFont);

    //players benches
    QRectF homeBench(62.0, -6.5, 34.0, 6.0);
    QRectF awayBench(104.0, -6.5, 34.0, 6.0);

    auto drawBench = [&](const QRectF &rect, const QColor &fillColor) {
        painter->setPen(QPen(QColor(60, 60, 60), 0.6));
        painter->setBrush(fillColor);
        painter->drawRoundedRect(rect, 1.0, 1.0);

        painter->setPen(QColor(40, 40, 40));
    };

    drawBench(homeBench, QColor(210, 225, 240));
    drawBench(awayBench, QColor(240, 220, 220));

    //penalty boxes
    QRectF homePenalty(74.0, rinkH + 0.5, 14.0, 5.5);
    QRectF scoreKeeper(91.0, rinkH + 0.5, 18.0, 5.5);
    QRectF awayPenalty(112.0, rinkH + 0.5, 14.0, 5.5);

    drawBench(homePenalty, QColor(235, 235, 235));
    drawBench(scoreKeeper, QColor(245, 245, 230));
    drawBench(awayPenalty, QColor(235, 235, 235));
}