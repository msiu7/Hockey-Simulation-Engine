#include "skater.hpp"

skater::skater(int number, std::string firstName, std::string lastName, int handedness, skaterOffensiveStats o, skaterDefensiveStats d): 
	player(number, firstName, lastName, handedness)
{
	off = o;
	def = d;
}

std::ostream& skater::showSpecificStats(std::ostream& out) const
{
	out << "OFFENSIVE: \nWrist Shot: " << off.wristShot << "\nSlap Shot: " << off.slapShot << "\nBackhand: " << off.backhand << "\nPassing:" << off.passing << "\nStickhandling: " << off.stickhandling << std::endl;
	out << "DEFENSIVE: \nFaceoffs: " << def.faceoffs << "\nChecking: " << def.checking << "\nDefensive Positioning: " << def.dPositioning << std::endl;
	return out;
}
