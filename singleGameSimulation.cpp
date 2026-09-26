#include "singleGameSimulation.hpp"
#include "ui_mainwindow.h"
#include "player.hpp"
#include "team.hpp"

singleGameSimulation::singleGameSimulation(Ui::MainWindow *uiptr){
    ui = uiptr;
    home = nullptr;
    away = nullptr;
    period = 1;
    clock = new QTimer();
    simClock = new QTimer();
    secondsRemainingInPeriod = 1200;
    teamDefendingLeft = nullptr;
    teamDefendingRight = nullptr;
    buildRinkBounds();
    thePuck = new puck(&world, 100, 42.5);
    cF.sim = this;
    world.SetContactFilter(&cF);
}

singleGameSimulation::singleGameSimulation(Ui::MainWindow *uiptr, team* a, team* h){
    ui = uiptr;
    home = h;
    away = a;
    period = 1;
    clock = new QTimer();
    simClock = new QTimer();
    secondsRemainingInPeriod = 1200;
    teamDefendingLeft = a;
    teamDefendingRight = h;
    buildRinkBounds();
    thePuck = new puck(&world, 100, 42.5);
    cF.sim = this;
    world.SetContactFilter(&cF);
}

QList<player*> singleGameSimulation::getAwayPlayers(){ return away ? away->lineup : QList<player*>(); }
QList<player*> singleGameSimulation::getHomePlayers(){ return home ? home->lineup : QList<player*>(); }

void singleGameSimulation::configureTimer(){
    clock->setInterval(1000); //1 second
    clock->setTimerType(Qt::PreciseTimer);
    simClock->setInterval(10); //1 second
    simClock->setTimerType(Qt::PreciseTimer);
    ui->periodLabel->setText("Period: " + QString::number(period));
    ui->timerLabel->setText(QString("Timer: %1:%2").arg(secondsRemainingInPeriod / 60, 2, 10, QChar('0')).arg(secondsRemainingInPeriod % 60, 2, 10, QChar('0')));

    //every tick, updateTimerDisplay will run
    QObject::connect(clock, &QTimer::timeout, [this]() {
        if(secondsRemainingInPeriod != 0) {secondsRemainingInPeriod--;}
        updateTimerDisplay();
    });

    connect(simClock, &QTimer::timeout, this, &singleGameSimulation::runSimulation);
}

void singleGameSimulation::configurePlayers(){
    qreal x = 65; qreal y = -3;
    for (player* p : away->lineup){
        p->setPosition(x, y);
        p->configurePlayerIcon(x, y, Qt::red);
        p->initPhysicsBody(&world, x, y);
        p->body->SetEnabled(false);
        p->body->SetTransform(p->body->GetPosition(), 3.14159265359f / 2.0f); // facing down
        emit p->rotationChanged();
        x += 2;
    }
    x = 107;
    for (player* p : home->lineup){
        p->setPosition(x, y);
        p->configurePlayerIcon(x, y, Qt::blue);
        p->initPhysicsBody(&world, x, y);
        p->body->SetEnabled(false);
        p->body->SetTransform(p->body->GetPosition(), 3.14159265359f / 2.0f); // facing down
        emit p->rotationChanged();
        x += 2;
    }

    emit playersChanged();
    emit puckChanged();
}

void singleGameSimulation::updateTimerDisplay(){
    ui->timerLabel->setText(QString("Timer: %1:%2").arg(secondsRemainingInPeriod / 60, 2, 10, QChar('0')).arg(secondsRemainingInPeriod % 60, 2, 10, QChar('0')));
}

void singleGameSimulation::updatePeriodDisplay(){
    period++;
    ui->periodLabel->setText("Period: " + QString::number(period));
    secondsRemainingInPeriod = 1200;
    updateTimerDisplay();
}

void singleGameSimulation::runSimulation(){    //this function is called on every tick of simClock

    world.Step(0.01f, 8, 3); //10 ms, velocity and position iterators smooth things out.

    for(player* p : away->playersOnIce){
        p->updatePhysics(0.01f);
        if(std::abs(p->tX - p->x) <= 1 && std::abs(p->tY - p->y) <= 1){ //tolerance of 2 ft.
            p->updateAngularPhysics(0.01f, thePuck->y - p->y, thePuck->x - p->x); //when at target, turn to face the puck
        }
    }

    for(player* p : home->playersOnIce){
        p->updatePhysics(0.01f);
        if(std::abs(p->tX - p->x) <= 1 && std::abs(p->tY - p->y) <= 1){ //tolerance of 2 ft.
            p->updateAngularPhysics(0.01f, thePuck->y - p->y, thePuck->x - p->x); //when at target, turn to face the puck
        }
    }

    thePuck->updatePhysics();
    //thePuck->getX();
    //thePuck->getY();

    //checkif puck should phase through
    if (puckPhasingActive) {
        // Disable phasing once the puck is, for example, 3+ feet away from the center dot (100, 42.5)
        float dx = thePuck->x - 100.0f;
        float dy = thePuck->y - 42.5f;
        if (std::hypot(dx, dy) > 3.5f) {
            puckPhasingActive = false;
            // Notify the fixture so Box2D checks collisions again immediately
            thePuck->puckBody->GetFixtureList()->Refilter();
        }
    }

    if (isStoppage){
        thePuck->puckBody->SetEnabled(false);
        if(isReadyForFaceOff()){
            faceOff();
        }
    }
    else{
        thePuck->puckBody->SetEnabled(true);
    }

}

void singleGameSimulation::playSim(){
    qDebug() << "playSim called!";
    if(!clock->isActive()){
        simClock->start();
    }
    if(away->playersOnIce.size() == 0){
        getPlayersOnIceToStartPeriod();
    }
    if(home->playersOnIce.size() == 0){
        getPlayersOnIceToStartPeriod();
    }
}

void singleGameSimulation::pauseSim(){
    if(clock->isActive()){
        clock->stop();
    }
    if(simClock->isActive()){
        simClock->stop();
    }
}

QString singleGameSimulation::timerText() const {
    return QString("%1:%2")
    .arg(secondsRemainingInPeriod / 60, 2, 10, QChar('0'))
        .arg(secondsRemainingInPeriod % 60, 2, 10, QChar('0'));
}

void singleGameSimulation::getPlayersOnIceToStartPeriod(){
    //for now assume both teams are starting with their first line every time (and no penalties)
    away->lineupMap[roles::LW1_5v5]->getOffBench(away);
    away->lineupMap[roles::C1_5v5]->getOffBench(away);
    awayCenterOnIce = away->lineupMap[roles::C1_5v5];
    away->lineupMap[roles::RW1_5v5]->getOffBench(away);
    away->lineupMap[roles::LD1_5v5]->getOffBench(away);
    away->lineupMap[roles::RD1_5v5]->getOffBench(away);
    away->lineupMap[roles::G1]->getOffBench(away);

    home->lineupMap[roles::LW1_5v5]->getOffBench(home);
    home->lineupMap[roles::C1_5v5]->getOffBench(home);
    homeCenterOnIce = home->lineupMap[roles::C1_5v5];
    home->lineupMap[roles::RW1_5v5]->getOffBench(home);
    home->lineupMap[roles::LD1_5v5]->getOffBench(home);
    home->lineupMap[roles::RD1_5v5]->getOffBench(home);
    home->lineupMap[roles::G1]->getOffBench(home);

    setUpFaceoffCenterIce();
}

void singleGameSimulation::buildRinkBounds() {
    b2BodyDef groundDef;
    groundDef.position.Set(0.0f, 0.0f);
    b2Body* rinkBody = world.CreateBody(&groundDef);

    b2FixtureDef fixtureDef;
    b2EdgeShape edge;
    fixtureDef.shape = &edge;
    fixtureDef.friction = 0.05f;
    fixtureDef.restitution = 0.35f;

    const float rinkW = 200.0f;
    const float rinkH = 85.0f;
    const float r = 28.0f;
    const int arcSegments = 12;
    const float PI = 3.14159265359f;

    // 1. Straight Board Edges
    // Top
    edge.SetTwoSided(b2Vec2(r, 0.0f), b2Vec2(rinkW - r, 0.0f));
    rinkBody->CreateFixture(&fixtureDef);
    // Bottom
    edge.SetTwoSided(b2Vec2(r, rinkH), b2Vec2(rinkW - r, rinkH));
    rinkBody->CreateFixture(&fixtureDef);
    // Left
    edge.SetTwoSided(b2Vec2(0.0f, r), b2Vec2(0.0f, rinkH - r));
    rinkBody->CreateFixture(&fixtureDef);
    // Right
    edge.SetTwoSided(b2Vec2(rinkW, r), b2Vec2(rinkW, rinkH - r));
    rinkBody->CreateFixture(&fixtureDef);

    // Helper lambda to generate two-sided arc edges
    auto buildCornerArc = [&](float cx, float cy, float startAngle, float endAngle) {
        for (int i = 0; i < arcSegments; ++i) {
            float a1 = startAngle + (static_cast<float>(i) / arcSegments) * (endAngle - startAngle); //increment angle
            float a2 = startAngle + (static_cast<float>(i + 1) / arcSegments) * (endAngle - startAngle);

            b2Vec2 p1(cx + r * std::cos(a1), cy + r * std::sin(a1)); //increment arc length
            b2Vec2 p2(cx + r * std::cos(a2), cy + r * std::sin(a2));

            edge.SetTwoSided(p1, p2);
            rinkBody->CreateFixture(&fixtureDef);
        }
    };

    // Top-Left (from PI to 1.5 * PI)
    buildCornerArc(r, r, PI, 1.5f * PI);

    // Top-Right (from 1.5 * PI to 2.0 * PI)
    buildCornerArc(rinkW - r, r, 1.5f * PI, 2.0f * PI);

    // Bottom-Right (from 0 to 0.5 * PI)
    buildCornerArc(rinkW - r, rinkH - r, 0.0f, 0.5f * PI);

    // Bottom-Left (from 0.5 * PI to PI)
    buildCornerArc(r, rinkH - r, 0.5f * PI, PI);

    //add goal cages
    buildGoalCageFixtures(rinkBody, rinkW - 11.0f, rinkH / 2.0f, false);
    buildGoalCageFixtures(rinkBody, 11.0f, rinkH / 2.0f, true);
}

void singleGameSimulation::buildGoalCageFixtures(b2Body* rinkBody, float goalLineX, float centerY, bool facingRight) {
    const float dir = facingRight ? -1.0f : 1.0f;
    const float halfW = 3.0f;           // 6 ft post-to-post
    const float depth = 3.33f;
    const float bookFlatHalfW = 2.0f;
    const float postRadius = 0.4f;

    const float topPostY = centerY - halfW;
    const float bottomPostY = centerY + halfW;
    const float backX = goalLineX + (dir * depth);

    // 1. Posts as dedicated static bodies (avoids compound-shape bounding box issues)
    b2CircleShape postShape;
    postShape.m_radius = postRadius;

    b2FixtureDef postDef;
    postDef.shape = &postShape;
    postDef.friction = 0.1f;
    postDef.restitution = 0.65f;

    b2BodyDef postBodyDef;
    postBodyDef.type = b2_staticBody;

    // Top Post
    postBodyDef.position.Set(goalLineX, topPostY);
    b2Body* topPost = world.CreateBody(&postBodyDef);
    topPost->CreateFixture(&postDef);

    // Bottom Post
    postBodyDef.position.Set(goalLineX, bottomPostY);
    b2Body* bottomPost = world.CreateBody(&postBodyDef);
    bottomPost->CreateFixture(&postDef);

    // 2. Net Frame Perimeter Edges
    b2FixtureDef frameDef;
    b2EdgeShape edge;
    frameDef.shape = &edge;
    frameDef.friction = 0.2f;
    frameDef.restitution = 0.15f;

    // Top outward flare
    b2Vec2 p0(goalLineX, topPostY);
    b2Vec2 p1(goalLineX + (dir * 0.8f), centerY - 3.52f);
    b2Vec2 p2(goalLineX + (dir * 2.1f), centerY - 3.40f);
    b2Vec2 p3(goalLineX + (dir * 3.05f), centerY - 2.75f);
    b2Vec2 pBackTop(backX, centerY - bookFlatHalfW);

    edge.SetTwoSided(p0, p1);        rinkBody->CreateFixture(&frameDef);
    edge.SetTwoSided(p1, p2);        rinkBody->CreateFixture(&frameDef);
    edge.SetTwoSided(p2, p3);        rinkBody->CreateFixture(&frameDef);
    edge.SetTwoSided(p3, pBackTop);   rinkBody->CreateFixture(&frameDef);

    // Straight flat back line
    b2Vec2 pBackBottom(backX, centerY + bookFlatHalfW);
    edge.SetTwoSided(pBackTop, pBackBottom);
    rinkBody->CreateFixture(&frameDef);

    // Bottom outward flare
    b2Vec2 p4(goalLineX + (dir * 3.05f), centerY + 2.75f);
    b2Vec2 p5(goalLineX + (dir * 2.1f), centerY + 3.40f);
    b2Vec2 p6(goalLineX + (dir * 0.8f), centerY + 3.52f);
    b2Vec2 p7(goalLineX, bottomPostY);

    edge.SetTwoSided(pBackBottom, p4); rinkBody->CreateFixture(&frameDef);
    edge.SetTwoSided(p4, p5);          rinkBody->CreateFixture(&frameDef);
    edge.SetTwoSided(p5, p6);          rinkBody->CreateFixture(&frameDef);
    edge.SetTwoSided(p6, p7);          rinkBody->CreateFixture(&frameDef);
}

void singleGameSimulation::setUpFaceoffCenterIce(){
    for(player* p : teamDefendingLeft->playersOnIce){
        if(p->evenStrengthPos == evenStrengthRoleTypes::LeftWing){p->setTargetPosition(97, 28);}
        else if(p->evenStrengthPos == evenStrengthRoleTypes::Center){p->setTargetPosition(98, 42.5);}
        else if(p->evenStrengthPos == evenStrengthRoleTypes::RightWing){p->setTargetPosition(97, 58);}
        else if(p->evenStrengthPos == evenStrengthRoleTypes::LeftDefense){p->setTargetPosition(88, 35);}
        else if(p->evenStrengthPos == evenStrengthRoleTypes::RightDefense){p->setTargetPosition(88, 51);}
        else{p->setTargetPosition(16, 42.5);}
    }
    for(player* p : teamDefendingRight->playersOnIce){
        if(p->evenStrengthPos == evenStrengthRoleTypes::LeftWing){p->setTargetPosition(103, 58);}
        else if(p->evenStrengthPos == evenStrengthRoleTypes::Center){p->setTargetPosition(102, 42.5);}
        else if(p->evenStrengthPos == evenStrengthRoleTypes::RightWing){p->setTargetPosition(103, 28);}
        else if(p->evenStrengthPos == evenStrengthRoleTypes::LeftDefense){p->setTargetPosition(112, 51);}
        else if(p->evenStrengthPos == evenStrengthRoleTypes::RightDefense){p->setTargetPosition(112, 35);}
        else{p->setTargetPosition(184, 42.5);}
    }
}

void singleGameSimulation::faceOff(){
    float randAngle = 0.0f;
    float randomProb = 0.0f;
    thePuck->puckBody->SetEnabled(true);
    puckPhasingActive = true;
    randomProb = getRandomAngle(0.0f, 100.0f);
    if(randomProb < 50.0f){randAngle = getRandomAngle(-3.14159265359f / 2.0f, 3.14159265359f / 2.0f);} //right side wins
    else{randAngle = getRandomAngle(3.14159265359f / 2.0f, 3.0f * 3.14159265359f / 2.0f);}
    thePuck->puckBody->SetLinearVelocity(b2Vec2(10.0f * std::cos(randAngle), 10.0f * std::sin(randAngle)));
    isStoppage = false;

    clock->start();
}

float singleGameSimulation::getRandomAngle(float min, float max){
    std::random_device rd;  // Obtains a seed from hardware
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine

    // 2. Define the range [lower_bound, upper_bound)
    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

bool singleGameSimulation::isReadyForFaceOff(){

    if (!teamDefendingLeft || !teamDefendingRight) return false;

    for(player* p : teamDefendingLeft->playersOnIce){
        if (!(std::abs(p->tX - p->x) <= 1 && std::abs(p->tY - p->y) <= 1 && std::abs(p->tAngle - p->angle) < .05f)) return false;
    }
    for(player* p : teamDefendingRight->playersOnIce){
        if (!(std::abs(p->tX - p->x) <= 1 && std::abs(p->tY - p->y) <= 1 && std::abs(p->tAngle - p->angle) < .05f)) return false;
    }
    return true;
}