#pragma once
#include <iostream>
#include <QString>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QGraphicsTextItem>
#include <QObject>
#include <QColor>
#include <QtQml/qqmlregistration.h>
#include "singleGameSimulation.hpp"


struct physicalStats {
	int strength = 50;
	int speed = 50;
	int acceleration = 50;
	int agility = 50;
	int stamina = 50;
};

struct mentalStats {
	int clutchness = 50;
	int leadership = 50;
	int toughness = 50;
	int riskiness = 50;
	int workRate = 50;
};

class player : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(float x READ getX NOTIFY positionChanged)
    Q_PROPERTY(float y READ getY NOTIFY positionChanged)
    Q_PROPERTY(int number READ getNumber CONSTANT)
    Q_PROPERTY(QString lastName READ getLastName CONSTANT)

protected:

	int number;
    QString firstName;
    QString lastName;
	int handedness; // 0 = left, 1 = right, 2 = default
    QString country;
	physicalStats phys;
	mentalStats ment;
    bool isDressed;
    bool isHighlighted = 0;
    QGraphicsEllipseItem* playerIcon;
    float tX = 0.0f;
    float tY = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float vX = 0.0f;
    float vY = 0.0f;

    // Movement tuning (in ft/s and ft/s^2)
    float maxSpeed = 28.0f;      // ~19 mph cruising/sprint mix
    float acceleration = 40.0f;  // ft/s^2 (takes ~0.7s to reach top speed)
    float iceFriction = 0.985f;  // Glide retention per tick (~1.5% drag)
    float brakeDecel = 60.0f;    // Active snow-plow / hockey stop braking

signals:
    void positionChanged();

public:

    player(int number = 0, QString firstName = "DEFAULT", QString lastName = "DEFAULT", int handedness = 2, QString country = "DEFAULT", bool isDressed = false);
    virtual ~player() = default;
	void setNumber(int num);
    void setFirstName(QString name);
    void setLastName(QString name);
	void setHandedness(int num);

	int getNumber() const { return number; }
    float getX() const {return x;}
    float getY() const {return y;}
    QString getFirstName() const { return firstName; }
    QString getLastName() const { return lastName; }
    QString getHandedness() const;
    QString getCountry() const { return country; }
	physicalStats& getPhysStats() { return phys; }
	mentalStats& getMentalStats() { return ment; }
    bool getIsDressed(){ return isDressed; }
    void configurePlayerIcon(qreal x, qreal y, QColor c);
	virtual std::ostream& showSpecificStats(std::ostream& out) const = 0;
    virtual QString getPositionAsString() const = 0;
    void setPosition(float newX, float newY);
    void getOnBench(singleGameSimulation* s);
    void getOffBench(singleGameSimulation* s);
    void updatePhysics(float dt);
    void setTargetPosition(float tX, float tY);
    friend class MainWindow;
    friend class singleGameSimulation;
};