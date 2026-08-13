#include "team.hpp"

int team::teamsCounter = 0;

team::team(QString city, QString teamName)
{
    this->city = city.simplified();
    this->teamName = teamName.simplified();
    for(int i = 0; i < 100; i++){
        this->numbersUsed[i] = false;
    }
    teamsCounter++;
}

team::~team(){
    for(player* p : roster){
        delete p;
    }
    teamsCounter--;
}
/*
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
*/
QString team::returnFullTeamName()
{
    return city + " " + teamName;
}
