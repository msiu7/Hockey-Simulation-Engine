#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
