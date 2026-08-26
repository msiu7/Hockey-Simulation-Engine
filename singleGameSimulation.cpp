#include "singleGameSimulation.hpp"

singleGameSimulation::singleGameSimulation(){
    r = new rink();
    //r->show();
}

singleGameSimulation::singleGameSimulation(team* a, team* h){
    home = h;
    away = a;
    r = new rink();
    //r->show();
}