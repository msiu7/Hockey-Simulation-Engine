#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QListWidget>
#include <QMessageBox>

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

private slots:
    void on_startSim_clicked();

    void on_editTeamsLineups_clicked();

    void on_backButton1_clicked();

    void on_backButton2_clicked();

    //lineup screen

    void on_addTeamButton_clicked();

    void onTeamSelected(QListWidgetItem* item);

    void on_backButton3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
