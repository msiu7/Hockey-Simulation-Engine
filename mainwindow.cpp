#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "team.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_editTeamsLineups_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_startSim_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_backButton1_clicked()
{
    generalBackButtonAction();
}

void MainWindow::on_backButton2_clicked()
{
    generalBackButtonAction();
}

void MainWindow::generalBackButtonAction()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_addTeamButton_clicked()
{
    team t = team(ui->cityLineEdit->text(), ui->teamNameLineEdit->text());
    QListWidgetItem *newTeam = new QListWidgetItem(t.returnFullTeamName());
    team::teamsCounter++;
    ui->teamsListWidget->addItem(newTeam);

    ui->numberOfTeamsLineEdit->setText(QString::number(team::getTeamsCounter()));
}

