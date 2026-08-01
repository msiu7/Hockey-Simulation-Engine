#include "skater.hpp"

skater::skater(int position, int number, QString firstName, QString lastName, int handedness, skaterOffensiveStats o, skaterDefensiveStats d):
	player(number, firstName, lastName, handedness)
{
	this->position = position;
	off = o;
	def = d;
}

std::ostream& skater::showSpecificStats(std::ostream& out) const
{
	out << "OFFENSIVE: \nWrist Shot: " << off.wristShot << "\nSlap Shot: " << off.slapShot << "\nBackhand: " << off.backhand << "\nPassing:" << off.passing << "\nStickhandling: " << off.stickhandling << std::endl;
	out << "DEFENSIVE: \nFaceoffs: " << def.faceoffs << "\nChecking: " << def.checking << "\nDefensive Positioning: " << def.dPositioning << std::endl;
	return out;
}

std::string skater::getPositionAsString() const
{
	if (position == 1) return "F";
	if (position == 2) return "D";
	return "DEFAULT";
}
