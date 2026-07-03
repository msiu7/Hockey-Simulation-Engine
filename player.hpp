#pragma once
#include <string>
#include <iostream>

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
	std::string firstName;
	std::string lastName;
	int handedness; // 0 = left, 1 = right, 2 = default
	physicalStats phys;
	mentalStats ment;

public:

	player(int number = 0, std::string firstName = "DEFAULT", std::string lastName = "DEFAULT", int handedness = 2);

	void setNumber(int num);
	void setFirstName(std::string name);
	void setLastName(std::string name);
	void setHandedness(int num);

	int getNumber() const { return number; }
	std::string getFirstName() const { return firstName; }
	std::string getLastName() const { return lastName; }
	std::string getHandedness() const;
	physicalStats& getPhysStats() { return phys; }
	mentalStats& getMentalStats() { return ment; }
	std::ostream& showUniversalStats(std::ostream& out) const;
	virtual std::ostream& showSpecificStats(std::ostream& out) const = 0;
	

	friend std::ostream& operator<<(std::ostream& out, const player& p);
};