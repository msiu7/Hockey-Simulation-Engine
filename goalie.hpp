#pragma once
#include <string>
#include <iostream>
#include "player.hpp"

struct goalieTechnicalStats {
	int glove;
	int blocker;
	int gPositioning;
	int reflexes;
}; 

class goalie : public player
{
private:
	goalieTechnicalStats goal;
public:
	goalie(int number = 0, std::string firstName = "DEFAULT", std::string lastName = "DEFAULT", int handedness = 2, goalieTechnicalStats g = {});
	goalieTechnicalStats& getGoalieStats() { return goal; }
	std::ostream& showSpecificStats(std::ostream& out) const override;
};