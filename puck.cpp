#include "puck.hpp"

puck::puck(b2World* world, float startX, float startY){
    x = startX;
    y = startY;

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(startX, startY);
    def.linearDamping = 0.15f;
    def.bullet = true;

    puckBody = world->CreateBody(&def); // Assigned to member variable

    b2CircleShape shape;
    shape.m_radius = 0.125f; // Sim units (feet)

    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = 0.25f;
    fixture.restitution = 0.65f;
    puckBody->CreateFixture(&fixture);

    emit positionChanged();
}

void puck::updatePhysics() {
    if (!puckBody) return;
    b2Vec2 pos = puckBody->GetPosition();
    if (pos.x != x || pos.y != y) {
        x = pos.x;
        y = pos.y;
        emit positionChanged();
    }
    b2Vec2 vel = puckBody->GetLinearVelocity();
    if (vel.x != vX || vel.y != vY) {
        vX = vel.x;
        vY = vel.y;
        emit positionChanged();
    }
}
