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

std::ostream& player::showUniversalStats(std::ostream& out) const
{
	out << "PHYSICAL: \nStrength: " << phys.strength << "\nSpeed: " << phys.speed << "\nAcceleration : " << phys.acceleration << "\nAgility : " << phys.acceleration << "\nStamina : " << phys.stamina << std::endl;
	out << "MENTAL: \nClutchness: " << ment.clutchness << "\nLeadership: " << ment.leadership << "\nToughness: " << ment.toughness << "\nRiskiness: " << ment.riskiness << "\nWork Rate: " << ment.workRate << std::endl;
	return out;
}

std::ostream& operator<<(std::ostream& out, const player& p)
{
	out << "Name: " << p.firstName << " " << p.lastName << std::endl << "Number: " << p.number << std::endl << "Handedness: " << p.getHandedness() << std::endl;
	p.showUniversalStats(out); 
	p.showSpecificStats(out);
	return out;
}
