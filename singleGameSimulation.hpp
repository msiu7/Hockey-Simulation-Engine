#pragma once
#include "team.hpp"
#include "rink.hpp"
#include "mainwindow.hpp"
#include <QTimer>
#include <QObject>

namespace Ui {
class MainWindow;
}

class singleGameSimulation{
private:
    Ui::MainWindow *ui; //to interact with the ui
    team* away;
    team* home;
    QTimer* clock;
    int period; //for now: periods 1, 2, and 3. 4 is OT, 5 is shootout.
    int secondsRemainingInPeriod;

public:
    singleGameSimulation(Ui::MainWindow *uiptr);
    singleGameSimulation(Ui::MainWindow *uiptr, team* a, team* h);
    ~singleGameSimulation() = default;
    void configureTimer();
    void updateTimerDisplay();
    void updatePeriodDisplay();
    void runSimulation();
    void playSim();
    void pauseSim();

    friend class MainWindow;
};
