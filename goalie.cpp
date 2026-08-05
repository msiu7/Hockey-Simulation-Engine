#include "goalie.hpp"

goalie::goalie(int number, QString firstName, QString lastName, int handedness, goalieTechnicalStats g):
	player(number, firstName, lastName, handedness)
{
	goal = g;
}

std::ostream& goalie::showSpecificStats(std::ostream& out) const
{
	out << "GOALIE: \nGlove: " << goal.glove << "\nBlocker: " << goal.blocker << "\nPositioning: " << goal.gPositioning << "\nReflexes: " << goal.reflexes << std::endl;
	return out;
}

QString goalie::getPositionAsString() const
{
	return "G";
}
