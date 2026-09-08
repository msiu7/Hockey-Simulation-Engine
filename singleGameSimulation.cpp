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
        x += 1.5;
    }
    x = 107;
    for (player* p : home->lineup){
        p->setPosition(x, y);
        p->configurePlayerIcon(x, y, Qt::blue);
        p->initPhysicsBody(&world, x, y);
        p->body->SetEnabled(false);
        x += 1.5; 
    }

    emit playersChanged();
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
        away->lineup[i]->setTargetPosition(100, -3);
    }

    for(int i = 0; i < 6; i++){
        home->lineup[i]->getOffBench(this);
        home->lineup[i]->setTargetPosition(0, 30);
    }

}

void singleGameSimulation::buildRinkBounds() {
    b2BodyDef groundDef;
    b2Body* groundBody = world.CreateBody(&groundDef);

    // Build the outer board perimeter (200x85 ft with 28 ft radius corners)
    // Box2D chain shape or simple 4 edges to start:
    b2EdgeShape edge;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &edge;
    fixtureDef.restitution = 0.4f; // Puck/skater bounce off boards
    fixtureDef.friction = 0.2f;

    // Top board
    edge.SetTwoSided(b2Vec2(28.0f, 0.0f), b2Vec2(172.0f, 0.0f));
    groundBody->CreateFixture(&fixtureDef);

    // Bottom board
    edge.SetTwoSided(b2Vec2(28.0f, 85.0f), b2Vec2(172.0f, 85.0f));
    groundBody->CreateFixture(&fixtureDef);

    // Left board
    edge.SetTwoSided(b2Vec2(0.0f, 28.0f), b2Vec2(0.0f, 57.0f));
    groundBody->CreateFixture(&fixtureDef);

    // Right board
    edge.SetTwoSided(b2Vec2(200.0f, 28.0f), b2Vec2(200.0f, 57.0f));
    groundBody->CreateFixture(&fixtureDef);
}

