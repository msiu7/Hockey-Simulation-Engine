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
    goalie(int number = 0, QString firstName = "DEFAULT", QString lastName = "DEFAULT", int handedness = 2, QString country = "DEFAULT", bool isDressed = false, goalieTechnicalStats g = {});
	goalieTechnicalStats& getGoalieStats() { return goal; }
	std::ostream& showSpecificStats(std::ostream& out) const override;
    QString getPositionAsString() const override;
    friend class MainWindow;
};