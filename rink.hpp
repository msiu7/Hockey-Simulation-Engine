#pragma once

#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>

class rink : public QGraphicsScene {
    Q_OBJECT

public:
    explicit rink(QObject *parent = nullptr);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void drawFaceoffCircleWithHashmarks(QPainter *painter, qreal cx, qreal cy, const QColor &color);
    void drawGoalCage(QPainter *painter, qreal goalLineX, qreal centerY, bool facingRight);
    void drawBenchesAndBoxes(QPainter *painter, qreal rinkW, qreal rinkH);
};