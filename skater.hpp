#pragma once
#include <string>
#include <iostream>
#include "player.hpp"

struct skaterOffensiveStats {
	int wristShot;
	int slapShot;
	int backhand;
	int passing;
	int stickhandling;
};

struct skaterDefensiveStats {
	int faceoffs;
	int checking;
	int dPositioning;
};

class skater : public player
{
private:
	skaterOffensiveStats off;
	skaterDefensiveStats def;
	int position; // 1 means forward, 2 means defenseman
public:
	skater(int position = 0, int number = 0, std::string firstName = "DEFAULT", std::string lastName = "DEFAULT", int handedness = 2, skaterOffensiveStats o = {}, skaterDefensiveStats d = {});
	skaterOffensiveStats& getOffStats() { return off; }
	skaterDefensiveStats& getDefStats() { return def; }
	std::ostream& showSpecificStats(std::ostream& out) const override;
	std::string getPositionAsString() const override;
};