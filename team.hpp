#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "player.hpp"
#include "skater.hpp"
#include "goalie.hpp"

class team
{
private:
	std::string city;
	std::string teamName;
	std::vector<std::unique_ptr<player>> roster;
	player* lineup[20];
public:
	team(std::string city = "NULL", std::string teamName = "NULL");
	void fillRoster();
	friend std::ostream& operator<<(std::ostream& out, const team& t);
};