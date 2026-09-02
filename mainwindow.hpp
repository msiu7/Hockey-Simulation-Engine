#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "team.hpp"
#include <QMainWindow>
#include <QListWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QCheckBox>
#include "player.hpp"
#include "singleGameSimulation.hpp"
#include <QQuickWidget>


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
    QTableWidgetItem* returnItemToPlace(player* p);
    void setMappingsFromTables();
    void refillTablesWithMappings();

    friend class singleGameSimulation;

private slots:
    void on_startSim_clicked();

    void on_editTeamsLineups_clicked();

    void on_backButton1_clicked();

    void on_backButton2_clicked();

    //lineup screen

    void on_addTeamButton_clicked();

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
    singleGameSimulation* currentSim = nullptr;
    team* awayTeamInSim = nullptr;
    team* homeTeamInSim = nullptr;
    QQuickWidget *qmlRink = nullptr;

    //tables in set lines
    static constexpr roles forward5v5Matrix[4][3] = {
        { roles::LW1_5v5, roles::C1_5v5, roles::RW1_5v5 },
        { roles::LW2_5v5, roles::C2_5v5, roles::RW2_5v5 },
        { roles::LW3_5v5, roles::C3_5v5, roles::RW3_5v5 },
        { roles::LW4_5v5, roles::C4_5v5, roles::RW4_5v5 }
    };

    static constexpr roles defense5v5Matrix[3][2] = {
        { roles::LD1_5v5, roles::RD1_5v5 },
        { roles::LD2_5v5, roles::RD2_5v5 },
        { roles::LD3_5v5, roles::RD3_5v5 }
    };

    static constexpr roles ppMatrix[2][5] = {
        { roles::LW1_PP, roles::C1_PP, roles::RW1_PP, roles::LD1_PP, roles::RD1_PP }, // PP Unit 1
        { roles::LW2_PP, roles::C2_PP, roles::RW2_PP, roles::LD2_PP, roles::RD2_PP }  // PP Unit 2
    };

    static constexpr roles pk4v5Matrix[2][4] = {
        { roles::W1_PK_4v5, roles::C1_PK_4v5, roles::LD1_PK_4v5, roles::RD1_PK_4v5 }, // PK Unit 1
        { roles::W2_PK_4v5, roles::C2_PK_4v5, roles::LD2_PK_4v5, roles::RD2_PK_4v5 }  // PK Unit 2
    };

    static constexpr roles pk3v5Matrix[2][3] = {
        { roles::C1_PK_3v5, roles::LD1_PK_3v5, roles::RD1_PK_3v5 }, // PK3 Unit 1
        { roles::C2_PK_3v5, roles::LD2_PK_3v5, roles::RD2_PK_3v5 }  // PK3 Unit 2
    };

    static constexpr roles even4v4Matrix[2][4] = {
        { roles::W1_4v4, roles::C1_4v4, roles::LD1_4v4, roles::RD1_4v4 }, // 4v4 Unit 1
        { roles::W2_4v4, roles::C2_4v4, roles::LD2_4v4, roles::RD2_4v4 }  // 4v4 Unit 2
    };
};

#endif // MAINWINDOW_HPP
