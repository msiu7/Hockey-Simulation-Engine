#include "customContactFilter.hpp"
#include "singleGameSimulation.hpp"
#include "player.hpp"
#include "puck.hpp"

bool customContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) {
    if (!sim || !sim->thePuck || !sim->thePuck->puckBody) {
        return b2ContactFilter::ShouldCollide(fixtureA, fixtureB);
    }

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    if (bodyA == sim->thePuck->puckBody || bodyB == sim->thePuck->puckBody) {
        b2Body* otherBody = (bodyA == sim->thePuck->puckBody) ? bodyB : bodyA;

        if (sim->puckPhasingActive) {
            if ((sim->awayCenterOnIce && otherBody == sim->awayCenterOnIce->body) ||
                (sim->homeCenterOnIce && otherBody == sim->homeCenterOnIce->body)) {
                return false;
            }
        }
    }

    return b2ContactFilter::ShouldCollide(fixtureA, fixtureB);
}