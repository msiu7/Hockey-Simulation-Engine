#include "player.hpp"
#include <string>

player::player(int number, std::string firstName, std::string lastName, int handedness)
{
	this->number = number;
	this->firstName = firstName;
	this->lastName = lastName;
	this->handedness = handedness;
}

void player::setNumber(int num)
{
	number = num;
}

void player::setFirstName(std::string name)
{
	firstName = name;
}

void player::setLastName(std::string name)
{
	lastName = name;
}

void player::setHandedness(int num)
{
	handedness = num;
}

std::string player::getHandedness() const
{
	if (handedness == 0) return "Left";
	if (handedness == 1) return "Right";
	return "DEFAULT";
}

std::ostream& operator<<(std::ostream& out, const player& p)
{
	out << "Name: " << p.firstName << " " << p.lastName << std::endl << "Number: " << p.number << std::endl << "Handedness: " << p.getHandedness();
	return out;
}
