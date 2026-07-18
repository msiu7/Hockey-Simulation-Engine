#include "team.hpp"

team::team(std::string city, std::string teamName)
{
	this->city = city;
	this->teamName = teamName;
}

void team::fillRoster()
{
	//forwards
	for (int i = 0; i < 14; i++) {
		roster.push_back(std::make_unique<skater>(1));
	}

	//defensemen
	for (int i = 0; i < 7; i++) {
		roster.push_back(std::make_unique<skater>(2));
	}

	//goalies
	for (int i = 0; i < 2; i++) {
		roster.push_back(std::make_unique<goalie>());
	}
}

std::ostream& operator<<(std::ostream& out, const team& t)
{
	for (int i = 0; i < t.roster.size(); i++) {
		out << *(t.roster[i]) << std::endl;
	}
	return out;
}
