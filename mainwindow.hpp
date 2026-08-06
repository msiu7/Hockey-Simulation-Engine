#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "team.hpp"
#include <QMainWindow>
#include <QListWidget>
#include <QMessageBox>
#include "player.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void generalBackButtonAction();
    void makePlayerListItem(player* p);
    void putPlayerAttributesToLineEdits(player* p);

private slots:
    void on_startSim_clicked();

    void on_editTeamsLineups_clicked();

    void on_backButton1_clicked();

    void on_backButton2_clicked();

    //lineup screen

    void on_addTeamButton_clicked();

    void onTeamSelected(QListWidgetItem* item);

    void on_backButton3_clicked();

    void on_playersListWidget_itemClicked(QListWidgetItem *item);

    void on_backButton4_clicked();

    void on_addCustomPlayerButton_clicked();

    void on_applyStatsChangesButton_clicked();

private:
    Ui::MainWindow *ui;
    team* currentTeam = nullptr;
    player* currentPlayer = nullptr;
};

#endif // MAINWINDOW_HPP
