#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "team.hpp"
#include <QMainWindow>
#include <QListWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QCheckBox>
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
    int getRandomNumber(int range1, int range2);
    player* generateRandomPlayerOfGivenPosition(int position);
    player* findHighlightedPlayer();
    QTableWidgetItem* returnItemToPlace();

private slots:
    void on_startSim_clicked();

    void on_editTeamsLineups_clicked();

    void on_backButton1_clicked();

    void on_backButton2_clicked();

    //lineup screen

    void on_addTeamButton_clicked();

    void onTeamSelected(QListWidgetItem* item);

    void on_backButton3_clicked();

    void on_backButton4_clicked();

    void on_addCustomPlayerButton_clicked();

    void on_applyStatsChangesButton_clicked();

    void on_addRandomPlayerButton_clicked();

    void on_fillRosterButton_clicked();

    void on_setLinesButton_clicked();

    void on_backButton5_clicked();

    void on_playersToPickList_itemClicked(QListWidgetItem *item);

    void on_forwards5v5TableWidget_cellClicked(int row, int column);

    void on_defense5v5TableWidget_cellClicked(int row, int column);

    void on_goalieTableWidget_cellClicked(int row, int column);

    void on_pk3v5TableWidget_cellClicked(int row, int column);

    void on_pk4v5TableWidget_cellClicked(int row, int column);

    void on_even4v4TableWidget_cellClicked(int row, int column);

    void on_ppTableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    team* currentTeam = nullptr;
    player* currentPlayer = nullptr;
};

#endif // MAINWINDOW_HPP
