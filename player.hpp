#pragma once
#include <string>
#include <iostream>
#include <QString>

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

class player
{
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

public:

    player(int number = 0, QString firstName = "DEFAULT", QString lastName = "DEFAULT", int handedness = 2, QString country = "DEFAULT", bool isDressed = false);
    virtual ~player() = default;
	void setNumber(int num);
    void setFirstName(QString name);
    void setLastName(QString name);
	void setHandedness(int num);

	int getNumber() const { return number; }
    QString getFirstName() const { return firstName; }
    QString getLastName() const { return lastName; }
    QString getHandedness() const;
    QString getCountry() const { return country; }
	physicalStats& getPhysStats() { return phys; }
	mentalStats& getMentalStats() { return ment; }
    bool getIsDressed(){ return isDressed; }
    //std::ostream& showUniversalStats(std::ostream& out) const;
	virtual std::ostream& showSpecificStats(std::ostream& out) const = 0;
    virtual QString getPositionAsString() const = 0;
    friend class MainWindow;
    //friend std::ostream& operator<<(std::ostream& out, const player& p);
};