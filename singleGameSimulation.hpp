#pragma once
#include "team.hpp"
#include "rink.hpp"

class singleGameSimulation{
private:
    team* away;
    team* home;
    rink* r;
public:
    singleGameSimulation();
    singleGameSimulation(team* a, team* h);
    ~singleGameSimulation() = default;
};
