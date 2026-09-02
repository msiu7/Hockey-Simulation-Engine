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

    Q_PROPERTY(qreal x READ getX NOTIFY positionChanged)
    Q_PROPERTY(qreal y READ getY NOTIFY positionChanged)
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
    qreal x{0.0};
    qreal y{0.0};

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
    qreal getX() const {return x;}
    qreal getY() const {return y;}
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
    void setPosition(qreal newX, qreal newY);
    friend class MainWindow;
    friend class singleGameSimulation;
};