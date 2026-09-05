#include "player.hpp"
#include <string>

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

void player::getOnBench(singleGameSimulation* s){
    setPosition(x, -3);
    s->playersOnIce.removeOne(this);
}

void player::getOffBench(singleGameSimulation* s){
    setPosition(x, 2);
    s->playersOnIce.push_back(this);
}

void player::updatePhysics(float dt) {
    qDebug() << "updatePhysics called!";
    qDebug() << "Target Pos: tX = " << tX << ", tY = " << tY;
    qDebug() << "CurrentPos: x = " << x << ", y = " << y;
    float dx = tX - x;
    float dy = tY - y;
    float distToTarget = std::hypot(dx, dy);

    // 1. Determine Desired Movement Direction
    if (distToTarget > 1.0f) { // Active Skating
        float dirX = dx / distToTarget;
        float dirY = dy / distToTarget;

        // Accelerate toward target direction
        vX += dirX * acceleration * dt;
        vY += dirY * acceleration * dt;

        // Cap to max top speed
        float currentSpeed = std::hypot(vX, vY);
        if (currentSpeed > maxSpeed) {
            vX = (vX / currentSpeed) * maxSpeed;
            vY = (vY / currentSpeed) * maxSpeed;
        }
    } else {
        // 2. Active Braking / Gliding when near the target
        float currentSpeed = std::hypot(vX, vY);

        if (currentSpeed > 0.05f) {
            // Apply drag/glide physics
            vX *= iceFriction;
            vY *= iceFriction;

            // Optional: apply stronger braking if stopping intentionally
            float drop = brakeDecel * dt;
            float newSpeed = std::max(0.0f, currentSpeed - drop);
            vX = (vX / currentSpeed) * newSpeed;
            vY = (vY/ currentSpeed) * newSpeed;
        } else {
            vX = 0.0f;
            vY = 0.0f;
        }
    }

    // 3. Integrate Position
    x += vX * dt;
    y += vY * dt;

    setPosition(x, y);
    emit positionChanged();
}

void player::setTargetPosition(float targetX, float targetY){
    tX = targetX;
    tY = targetY;
}
