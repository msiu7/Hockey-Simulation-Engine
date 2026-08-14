#include "skater.hpp"

skater::skater(int position, int number, QString firstName, QString lastName, int handedness, QString country, bool isDressed, skaterOffensiveStats o, skaterDefensiveStats d):
    player(number, firstName, lastName, handedness, country, isDressed)
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

QString skater::getPositionAsString() const
{
	if (position == 1) return "F";
	if (position == 2) return "D";
	return "DEFAULT";
}
