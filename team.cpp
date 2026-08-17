#include "team.hpp"

int team::teamsCounter = 0;
int team::numDressed = 0;

team::team(QString city, QString teamName)
{
    this->city = city.simplified();
    this->teamName = teamName.simplified();
    for(int i = 0; i < 100; i++){
        this->numbersUsed[i] = false;
    }
    teamsCounter++;

    this->lineupMap = { //declaring all roles as null to start
        //even strength
        {roles::LW1_5v5, nullptr}, {roles::C1_5v5, nullptr}, {roles::RW1_5v5, nullptr}, {roles::LD1_5v5, nullptr}, {roles::RD1_5v5, nullptr},
        {roles::LW2_5v5, nullptr}, {roles::C2_5v5, nullptr}, {roles::RW2_5v5, nullptr}, {roles::LD2_5v5, nullptr}, {roles::RD2_5v5, nullptr},
        {roles::LW3_5v5, nullptr}, {roles::C3_5v5, nullptr}, {roles::RW3_5v5, nullptr}, {roles::LD3_5v5, nullptr}, {roles::RD3_5v5, nullptr},
        {roles::LW4_5v5, nullptr}, {roles::C4_5v5, nullptr}, {roles::RW4_5v5, nullptr}, {roles::G1, nullptr}, {roles::G2, nullptr},

        //pp
        {roles::LW1_PP, nullptr}, {roles::C1_PP, nullptr}, {roles::RW1_PP, nullptr}, {roles::LD1_PP, nullptr}, {roles::RD1_PP, nullptr},
        {roles::LW2_PP, nullptr}, {roles::C2_PP, nullptr}, {roles::RW2_PP, nullptr}, {roles::LD2_PP, nullptr}, {roles::RD2_PP, nullptr},

        //pk4v5
        {roles::W1_PK_4v5, nullptr}, {roles::C1_PK_4v5, nullptr}, {roles::LD1_PK_4v5, nullptr}, {roles::RD1_PK_4v5, nullptr},
        {roles::W2_PK_4v5, nullptr}, {roles::C2_PK_4v5, nullptr}, {roles::LD2_PK_4v5, nullptr}, {roles::RD2_PK_4v5, nullptr},

        //pk3v5
        {roles::C1_PK_3v5, nullptr}, {roles::LD1_PK_3v5, nullptr}, {roles::RD1_PK_3v5, nullptr},
        {roles::C2_PK_3v5, nullptr}, {roles::LD2_PK_3v5, nullptr}, {roles::RD2_PK_3v5, nullptr},

        //4v4
        {roles::W1_4v4, nullptr}, {roles::C1_4v4, nullptr}, {roles::LD1_4v4, nullptr}, {roles::RD1_4v4, nullptr},
        {roles::W2_4v4, nullptr}, {roles::C2_4v4, nullptr}, {roles::LD2_4v4, nullptr}, {roles::RD2_4v4, nullptr}
    };
}

team::~team(){
    for(player* p : roster){
        delete p;
    }
    teamsCounter--;
}

QString team::returnFullTeamName()
{
    return city + " " + teamName;
}
