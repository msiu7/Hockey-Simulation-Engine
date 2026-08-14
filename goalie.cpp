#include "goalie.hpp"

goalie::goalie(int number, QString firstName, QString lastName, int handedness, QString country, bool isDressed, goalieTechnicalStats g):
    player(number, firstName, lastName, handedness, country, isDressed)
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
