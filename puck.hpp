#pragma once

#include <QObject>
#include <QQmlEngine>
#include <box2d/box2d.h>

class puck : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float x READ getX NOTIFY positionChanged)
    Q_PROPERTY(float y READ getY NOTIFY positionChanged)

private:
    b2Body* puckBody = nullptr;
    float tX = 0.0f;
    float tY = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float vX = 0.0f;
    float vY = 0.0f;
public:
    puck(b2World* world, float startX, float startY);
    float getX() const {qDebug() << "puck::getX called, returning:" << x; return x;}
    float getY() const {qDebug() << "puck::getY called, returning:" << y; return y;}
    void updatePhysics();
    float setX(float newX){x = newX;}
    float setY(float newY){y = newY;}

signals:
    void positionChanged();

    friend class singleGameSimulation;
};


