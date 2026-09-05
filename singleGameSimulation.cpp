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
        //p->setTargetPosition(x, y);
        p->configurePlayerIcon(x, y, Qt::red);
        x += 1.5;
    }
    x = 107;
    for (player* p : home->lineup){
        p->setPosition(x, y);
        //p->setTargetPosition(x, y);
        p->configurePlayerIcon(x, y, Qt::blue);
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

void singleGameSimulation::runSimulation(){ //this function is called on every tick of simClock
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
        away->lineup[i]->setTargetPosition(i * 20, i * 5);
    }

    for(int i = 0; i < 6; i++){
        home->lineup[i]->getOffBench(this);
        home->lineup[i]->setTargetPosition(i * 20 + 5, i * 5 + 2);
    }

}

