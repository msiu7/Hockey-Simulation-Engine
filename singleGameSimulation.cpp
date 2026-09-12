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
    thePuck = new puck(&world, 100, 50);
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
    thePuck = new puck(&world, 100, 50);
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

    for(player* p : playersOnIce){
        p->updatePhysics(0.01f);
    }

    thePuck->updatePhysics();
    thePuck->getX();
    thePuck->getY();

    if (isStoppage){
    }
    else{

    }

}

void singleGameSimulation::playSim(){
    qDebug() << "playSim called!";
    if(!clock->isActive()){
        simClock->start();
    }
    if(playersOnIce.size() == 0){
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
    for(int i = 0; i < 6; i++){
        away->lineup[i]->getOffBench(this);
        away->lineup[i]->setTargetPosition(0, 0);
    }

    for(int i = 0; i < 6; i++){
        home->lineup[i]->getOffBench(this);
        home->lineup[i]->setTargetPosition(200, 85);
    }

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
}
