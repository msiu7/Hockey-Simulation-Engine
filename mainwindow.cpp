#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "team.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->teamsListWidget, &QListWidget::itemClicked, this, &MainWindow::onTeamSelected);

    QStringList positions = {"F", "D", "G"};
    ui->positionComboBox->addItems(positions);

    QStringList handednesses = {"L", "R"};
    ui->handednessComboBox->addItems(handednesses);
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
    //check input validity
    if(ui->cityLineEdit->text().simplified().isEmpty() || ui->teamNameLineEdit->text().simplified().isEmpty()){
        QMessageBox::warning(this, "Input Error", "City and/or team name fields are empty.");
        return;
    }
    if(ui->teamsListWidget->findItems(ui->cityLineEdit->text().simplified() + " " + ui->teamNameLineEdit->text().simplified(), Qt::MatchContains).count() > 0){
        QMessageBox::warning(this, "Input Error", "This team already exists.");
        return;
    }

    //adding list widget item
    team* t = new team(ui->cityLineEdit->text(), ui->teamNameLineEdit->text());

    QListWidgetItem *newTeam = new QListWidgetItem(t->returnFullTeamName());

    //makes team pointer retrievable later
    newTeam->setData(Qt::UserRole, QVariant::fromValue(t));

    ui->teamsListWidget->addItem(newTeam);

    //setting row format
    QWidget* rowWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(rowWidget);
    QPushButton* deleteTeam = new QPushButton();
    deleteTeam->setIcon(deleteTeam->style()->standardIcon(QStyle::SP_TrashIcon));

    layout->addStretch();
    layout->addWidget(deleteTeam);

    //attach layout to listItem
    ui->teamsListWidget->setItemWidget(newTeam, rowWidget);

    ui->numberOfTeamsLineEdit->setText(QString::number(team::getTeamsCounter()));

    //deletion mechanic
    //in the lambda: "this" is the list widget (row)
    QObject::connect(deleteTeam, &QPushButton::clicked, [this, newTeam, t]() {
        int row = ui->teamsListWidget->row(newTeam);
        if (row != -1) { //if the row exists
            delete ui->teamsListWidget->takeItem(row);
            delete t;
            ui->numberOfTeamsLineEdit->setText(QString::number(team::getTeamsCounter()));
        }
    });
}

void MainWindow::onTeamSelected(QListWidgetItem* item)
{
    if (!item) return;

    // Retrieving team pointer
    this->currentTeam = item->data(Qt::UserRole).value<team*>();

    ui->playersListWidget->clear();

    // 2. RE-POPULATE only with the newly selected team's players
    for (player* p : this->currentTeam->roster) {
        QListWidgetItem* temp = new QListWidgetItem(p->getFirstName() + " " + p->getLastName());

        // Save Player Pointer
        item->setData(Qt::UserRole, QVariant::fromValue(p));
        ui->playersListWidget->addItem(temp);
    }

    ui->teamNameLabel2->setText(this->currentTeam->returnFullTeamName());
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_backButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_addCustomSkaterButton_clicked()
{
    //check input validity
    if(ui->positionComboBox->currentText().isEmpty() || ui->handednessComboBox->currentText().isEmpty() || ui->numberLineEdit->text().simplified().isEmpty() || ui->firstNameLineEdit->text().simplified().isEmpty() || ui->lastNameLineEdit->text().simplified().isEmpty()){
        QMessageBox::warning(this, "Input Error", "Some fields are empty.");
        return;
    }
    if(ui->playersListWidget->findItems(ui->firstNameLineEdit->text().simplified() + " " + ui->lastNameLineEdit->text().simplified(), Qt::MatchContains).count() > 0){
        QMessageBox::warning(this, "Input Error", "This player already exists.");
        return;
    }

    //creating a player
    int handednessTemp;
    int positionTemp;
    if(ui->handednessComboBox->currentText() == "L")
        handednessTemp = 0;
    else
        handednessTemp = 1;

    if(ui->positionComboBox->currentText() == "G")
        player* p = new goalie(ui->numberLineEdit->text().simplified().toInt(), ui->firstNameLineEdit->text().simplified(), ui->lastNameLineEdit->text().simplified(), handednessTemp);
    else
        if(ui->positionComboBox->currentText() == "F")
            positionTemp = 1;
        else
            positionTemp = 2;
        player* p = new skater(positionTemp, ui->numberLineEdit->text().simplified().toInt(), ui->firstNameLineEdit->text().simplified(), ui->lastNameLineEdit->text().simplified(), handednessTemp);

        QListWidgetItem *newPlayer = new QListWidgetItem(p->getFirstName() + " " + p->getLastName());
        this->currentTeam->roster.push_back(p);

        ui->playersListWidget->addItem(newPlayer);
}

