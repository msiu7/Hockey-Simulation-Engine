#include "singleGameSimulation.hpp"
#include "ui_mainwindow.h"

singleGameSimulation::singleGameSimulation(Ui::MainWindow *uiptr){
    ui = uiptr;
    home = nullptr;
    away = nullptr;
    period = 1;
    clock = new QTimer();
    simClock = new QTimer();
    secondsRemainingInPeriod = 1200;
}

singleGameSimulation::singleGameSimulation(Ui::MainWindow *uiptr, team* a, team* h){
    ui = uiptr;
    home = h;
    away = a;
    period = 1;
    clock = new QTimer();
    simClock = new QTimer();
    secondsRemainingInPeriod = 1200;
}

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
        x += 1.5;
    }
    x = 107;
    for (player* p : home->lineup){
        p->setPosition(x, y);
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

void singleGameSimulation::runSimulation(){

    static bool toggle = false;
    if(toggle) away->lineup[0]->setPosition(100, 60);
    else away->lineup[0]->setPosition(30, 30);
    toggle = !toggle;

}

void singleGameSimulation::playSim(){
    if(!clock->isActive()){
        clock->start();
        simClock->start();
    }
}

void singleGameSimulation::pauseSim(){
    if(clock->isActive()){
        clock->stop();
    }
}

QString singleGameSimulation::timerText() const {
    return QString("%1:%2")
    .arg(secondsRemainingInPeriod / 60, 2, 10, QChar('0'))
        .arg(secondsRemainingInPeriod % 60, 2, 10, QChar('0'));
}

