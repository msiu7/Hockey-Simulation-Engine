#include "player.hpp"
#include "team.hpp"
#include <string>

class team;

player::player(int number, QString firstName, QString lastName, int handedness, QString country, bool isDressed)
{
	this->number = number;
	this->firstName = firstName;
	this->lastName = lastName;
	this->handedness = handedness;
    this->country = country;
    this->isDressed = isDressed;
    playerIcon = new QGraphicsEllipseItem(-1.5, -1.5, 3, 3);
}

void player::setNumber(int num)
{
	number = num;
}

void player::setFirstName(QString name)
{
    firstName = name.simplified();
}

void player::setLastName(QString name)
{
    lastName = name.simplified();
}

void player::setHandedness(int num)
{
	handedness = num;
}

QString player::getHandedness() const
{
    if (handedness == 0) return "L";
    if (handedness == 1) return "R";
	return "DEFAULT";
}

void player::configurePlayerIcon(qreal x, qreal y, QColor c){
    playerIcon->setBrush(QBrush(c));
    playerIcon->setPen(QPen(Qt::black, 0)); //one pixel
    playerIcon->setPos(x, y);

    QGraphicsTextItem *numberIcon = new QGraphicsTextItem(QString::number(number), playerIcon);

    //set font size
    QFont font = numberIcon->font();
    font.setPointSizeF(1.5);

    numberIcon->setFont(font);

    //center the number
    x = ((playerIcon->rect().width() - numberIcon->boundingRect().width()) / 2) - 1.5;
    y = ((playerIcon->rect().height() - numberIcon->boundingRect().height()) / 2) - 3.75;
    numberIcon->setPos(x, y);

}

void player::setPosition(float newX, float newY) {
    if (x != newX || y != newY) {
        x = newX;
        y = newY;
        emit positionChanged();
    }
}

void player::getOnBench(team* t){
    setPosition(x, -3);
    t->playersOnIce.removeOne(this);

    body->SetTransform(b2Vec2(x, -3), 3.14159265359f / 2.0f);
    body->SetEnabled(false);
}

void player::getOffBench(team* t){
    setPosition(x, 2);
    t->playersOnIce.push_back(this);

    body->SetTransform(b2Vec2(x, 2), body->GetAngle());
    body->SetEnabled(true);
}

void player::updatePhysics(float dt) {
    if (!body) return;

    b2Vec2 currentPos = body->GetPosition();
    float dx = tX - currentPos.x;
    float dy = tY - currentPos.y;
    float dist = std::hypot(dx, dy);

    b2Vec2 targetVel(0.0f, 0.0f);

    if (dist > 1.0f) {
        updateAngularPhysics(dt, dy, dx);
        float dirX = dx / dist;
        float dirY = dy / dist;
        targetVel.Set(dirX * maxSpeed, dirY * maxSpeed);
    }

    // Apply smooth acceleration toward desired velocity
    b2Vec2 currentVel = body->GetLinearVelocity();
    b2Vec2 velDiff = targetVel - currentVel;
    b2Vec2 force = (body->GetMass() / dt) * velDiff;

    // Cap the maximum skating push force
    float maxForce = body->GetMass() * acceleration;
    if (force.Length() > maxForce) {
        force.Normalize();
        force *= maxForce;
    }

    body->ApplyForceToCenter(force, true);

    // Sync back to member variables for QML bindings
    x = body->GetPosition().x;
    y = body->GetPosition().y;

    if (playerIcon) {
        playerIcon->setPos(x, y);
    }
    emit positionChanged();
}

void player::setTargetPosition(float targetX, float targetY){
    tX = targetX;
    tY = targetY;
}

void player::initPhysicsBody(b2World* world, float startX, float startY) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(startX, startY);
    bodyDef.linearDamping = 0.5f; // Mimic ice gliding drag
    bodyDef.fixedRotation = true;  // Keep players upright

    body = world->CreateBody(&bodyDef);

    b2CircleShape circle;
    circle.m_radius = 1.0f; // ~1 ft collision radius for a skater

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 27.3f; // 27.3(density) * pi *(1)^2(area) * 2.2(kg to lbs) is about 190 lb skaters.
    fixtureDef.friction = 0.1f;      // Low friction for ice
    fixtureDef.restitution = 0.2f;   // Soft bounce on impact
    body->CreateFixture(&fixtureDef);
}

void player::facePoint(float targetX, float targetY) {
    if (!body) return;
    b2Vec2 pos = body->GetPosition();
    angle = std::atan2(targetY - pos.y, targetX - pos.x);
    body->SetTransform(pos, angle);
    emit rotationChanged();
}

void player::updateAngularPhysics(float dt, float dyToFace, float dxToFace){
    tAngle = std::atan2(dyToFace, dxToFace);
    angle = body->GetAngle();

    // 2. Shortest rotational difference (-PI to +PI)
    float diff = tAngle - angle;
    while (diff < -3.14159265f) diff += 2.0f * 3.14159265f;
    while (diff >  3.14159265f) diff -= 2.0f * 3.14159265f;

    // 3. Max turn speed in radians per second (e.g., 5.0 rad/s ~ 286 deg/s)
    const float maxTurnRate = 6.0f;
    float maxStep = maxTurnRate * dt;

    // Clamp the rotation step for this frame
    float step = std::clamp(diff, -maxStep, maxStep);
    if(std::abs(diff) <= maxStep){angle = tAngle;}
    else{angle += step;}

    body->SetTransform(body->GetPosition(), angle);
    emit rotationChanged();
    qDebug() << "Angle: " << angle;
}