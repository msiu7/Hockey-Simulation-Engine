#pragma once
#include <box2d/box2d.h>

class singleGameSimulation;

class customContactFilter : public b2ContactFilter {
public:
    singleGameSimulation* sim = nullptr;

    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;
};