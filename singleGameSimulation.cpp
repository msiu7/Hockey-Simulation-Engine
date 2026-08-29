#include "singleGameSimulation.hpp"
#include "ui_mainwindow.h"

singleGameSimulation::singleGameSimulation(Ui::MainWindow *uiptr){
    ui = uiptr;
    home = nullptr;
    away = nullptr;
    period = 1;
    clock = new QTimer();
    secondsRemainingInPeriod = 1200;
    r = new rink();
}

singleGameSimulation::singleGameSimulation(Ui::MainWindow *uiptr, team* a, team* h){
    ui = uiptr;
    home = h;
    away = a;
    period = 1;
    clock = new QTimer();
    secondsRemainingInPeriod = 1200;
    r = new rink();
}

void singleGameSimulation::configureTimer(){
    clock->setInterval(1000); //1 second
    clock->setTimerType(Qt::PreciseTimer);
    ui->periodLabel->setText("Period: " + QString::number(period));
    ui->timerLabel->setText(QString("Timer: %1:%2").arg(secondsRemainingInPeriod / 60, 2, 10, QChar('0')).arg(secondsRemainingInPeriod % 60, 2, 10, QChar('0')));

    //every tick, updateTimerDisplay will run
    QObject::connect(clock, &QTimer::timeout, [this]() {
        if(secondsRemainingInPeriod != 0) {secondsRemainingInPeriod--;}
        updateTimerDisplay();
    });
}

void singleGameSimulation::configurePlayers(){
    qreal x = 0; qreal y = 0;
    for (player* p : away->lineup){
        p->configurePlayerIcon(x, y, Qt::red);
        x += 5;
        y += 5;
        r->addItem(p->playerIcon);
    }
    for (player* p : home->lineup){
        p->configurePlayerIcon(x, y, Qt::blue);
        x += 5;
        y += 5;
        r->addItem(p->playerIcon);
    }
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

}

void singleGameSimulation::playSim(){
    if(!clock->isActive()){
        clock->start();
    }
}

void singleGameSimulation::pauseSim(){
    if(clock->isActive()){
        clock->stop();
    }
}