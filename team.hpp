#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "player.hpp"
#include "skater.hpp"
#include "goalie.hpp"
#include <QString>
#include <unordered_map>

enum class roles{
    LW1_5v5, C1_5v5, RW1_5v5, LD1_5v5, RD1_5v5,
    LW2_5v5, C2_5v5, RW2_5v5, LD2_5v5, RD2_5v5,
    LW3_5v5, C3_5v5, RW3_5v5, LD3_5v5, RD3_5v5,
    LW4_5v5, C4_5v5, RW4_5v5, G1, G2,

    LW1_PP, C1_PP, RW1_PP, LD1_PP, RD1_PP,
    LW2_PP, C2_PP, RW2_PP, LD2_PP, RD2_PP,

    W1_PK_4v5, C1_PK_4v5, LD1_PK_4v5, RD1_PK_4v5,
    W2_PK_4v5, C2_PK_4v5, LD2_PK_4v5, RD2_PK_4v5,

    C1_PK_3v5, LD1_PK_3v5, RD1_PK_3v5,
    C2_PK_3v5, LD2_PK_3v5, RD2_PK_3v5,

    W1_4v4, C1_4v4, LD1_4v4, RD1_4v4,
    W2_4v4, C2_4v4, LD2_4v4, RD2_4v4,
};

class team
{
private:
    QString city;
    QString teamName;
    std::vector<player*> roster;
    std::vector<player*> lineup;
    bool numbersUsed[100]{};
    static int teamsCounter;
    static int numDressed;
    std::unordered_map<roles, player*> lineupMap;
public:
    team(QString city = "NULL", QString teamName = "NULL");
    ~team();
	void fillRoster();
    QString returnFullTeamName();
    static int getTeamsCounter(){ return teamsCounter; }
    friend std::ostream& operator<<(std::ostream& out, const team& t);

    friend class MainWindow;
    friend class singleGameSimulation;

};