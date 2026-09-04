#pragma once

#include <QObject>
#include <QTimer>
#include <QtQml/qqmlregistration.h>
#include "team.hpp"
#include "player.hpp"

namespace Ui {
class MainWindow;
}

class singleGameSimulation : public QObject{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int period READ getPeriod NOTIFY periodChanged)
    Q_PROPERTY(QList<player*> awayPlayers READ getAwayPlayers NOTIFY playersChanged)
    Q_PROPERTY(QList<player*> homePlayers READ getHomePlayers NOTIFY playersChanged)

private:
    Ui::MainWindow *ui; //to interact with the ui
    team* away;
    team* home;
    QTimer* clock;
    QTimer* simClock;
    int period; //for now: periods 1, 2, and 3. 4 is OT, 5 is shootout.
    int secondsRemainingInPeriod;

signals:
    void periodChanged();
    void playersChanged();

public:
    singleGameSimulation(Ui::MainWindow *uiptr);
    singleGameSimulation(Ui::MainWindow *uiptr, team* a, team* h);
    ~singleGameSimulation() = default;
    void configureTimer();
    void updateTimerDisplay();
    void updatePeriodDisplay();
    void runSimulation();
    Q_INVOKABLE void playSim();
    Q_INVOKABLE void pauseSim();
    void configurePlayers();
    int getPeriod() const { return period; }
    QString timerText() const;

    QList<player*> getAwayPlayers() const { return away ? away->lineup : QList<player*>(); }
    QList<player*> getHomePlayers() const { return home ? home->lineup : QList<player*>(); }

    friend class MainWindow;
};
