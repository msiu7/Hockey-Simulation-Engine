#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "player.hpp"
#include "skater.hpp"
#include "goalie.hpp"
#include <QString>

class team
{
private:
    QString city;
    QString teamName;
    std::vector<player*> roster;
	player* lineup[20];
    static int teamsCounter;
public:
    team(QString city = "NULL", QString teamName = "NULL");
    ~team();
	void fillRoster();
    QString returnFullTeamName();
    static int getTeamsCounter(){ return teamsCounter; }
    friend std::ostream& operator<<(std::ostream& out, const team& t);

    friend class MainWindow;

};