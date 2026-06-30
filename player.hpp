#pragma once
#include <string>
#include <iostream>

class player
{
private:

	int number;
	std::string firstName;
	std::string lastName;
	int handedness; // 0 = left, 1 = right, 2 = default

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

	friend std::ostream& operator<<(std::ostream& out, const player& p);
};