#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "team.hpp"
#include <vector>

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

    // ignore clicks on playersRowWidget (defer to the item itself)
    rowWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // ensure the trash button still gets clicks:
    deleteTeam->setAttribute(Qt::WA_TransparentForMouseEvents, false);

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
    ui->numberOfPlayersLineEdit->setText(QString::number(currentTeam->roster.size()));
    // reload selected team's roster
    for (player* p : this->currentTeam->roster) {
        makePlayerListItem(p);
    }

    ui->teamNameLabel2->setText(this->currentTeam->returnFullTeamName());
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_backButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::makePlayerListItem(player* p){
    QListWidgetItem *newPlayer = new QListWidgetItem(p->getFirstName() + " " + p->getLastName());

    //save player pointer
    newPlayer->setData(Qt::UserRole, QVariant::fromValue(p));

    ui->playersListWidget->addItem(newPlayer);

    //setting row format (same as team view basically)
    QWidget* playersRowWidget = new QWidget();
    QHBoxLayout* playersLayout = new QHBoxLayout(playersRowWidget);
    QPushButton* deletePlayer = new QPushButton();
    QLabel* num = new QLabel();
    QLabel* pos = new QLabel();
    QLabel* handedness = new QLabel();
    deletePlayer->setIcon(deletePlayer->style()->standardIcon(QStyle::SP_TrashIcon));
    num->setText(QString::number(p->getNumber()));
    pos->setText(p->getPositionAsString());
    handedness->setText(p->getHandedness());
    // ignore clicks on playersRowWidget (defer to the item itself)
    playersRowWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // ensure the trash button still gets clicks:
    deletePlayer->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    playersLayout->addStretch();
    playersLayout->addWidget(num);
    playersLayout->addStretch();
    playersLayout->addWidget(pos);
    playersLayout->addStretch();
    playersLayout->addWidget(handedness);
    playersLayout->addStretch();
    playersLayout->addWidget(deletePlayer);

    //attach layout to listItem
    ui->playersListWidget->setItemWidget(newPlayer, playersRowWidget);

    ui->numberOfPlayersLineEdit->setText(QString::number(currentTeam->roster.size()));

    //deletion mechanic
    //in the lambda: "this" is the list widget (row)
    QObject::connect(deletePlayer, &QPushButton::clicked, [this, newPlayer, p]() {
        int row = ui->playersListWidget->row(newPlayer);
        if (row != -1) { //if the row exists
            delete ui->playersListWidget->takeItem(row);
            std::erase(currentTeam->roster, p);
            delete p;
            ui->numberOfPlayersLineEdit->setText(QString::number(currentTeam->roster.size()));
        }});

}
void MainWindow::on_playersListWidget_itemClicked(QListWidgetItem *item)
{
    if (!item) return;

    // Retrieving player pointer
    this->currentPlayer = item->data(Qt::UserRole).value<player*>();

    ui->playerNameLabel->setText(this->currentPlayer->getFirstName() + " " + this->currentPlayer->getLastName());
    putPlayerAttributesToLineEdits(this->currentPlayer);
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_backButton4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_addCustomPlayerButton_clicked()
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
    player* p = nullptr;

    if(ui->positionComboBox->currentText() == "G")
    {
        p = new goalie(ui->numberLineEdit->text().simplified().toInt(), ui->firstNameLineEdit->text().simplified(), ui->lastNameLineEdit->text().simplified(), handednessTemp);
    }
    else{
        if(ui->positionComboBox->currentText() == "F")
            positionTemp = 1;
        else
            positionTemp = 2;
        p = new skater(positionTemp, ui->numberLineEdit->text().simplified().toInt(), ui->firstNameLineEdit->text().simplified(), ui->lastNameLineEdit->text().simplified(), handednessTemp);
    }
    this->currentTeam->roster.push_back(p);
    makePlayerListItem(p);
}

void MainWindow::putPlayerAttributesToLineEdits(player* p)
{
    ui->accelerationEdit->setText(QString::number(p->phys.acceleration));
    ui->speedEdit->setText(QString::number(p->phys.speed));
    ui->strengthEdit->setText(QString::number(p->phys.strength));
    ui->staminaEdit->setText(QString::number(p->phys.stamina));
    ui->agilityEdit->setText(QString::number(p->phys.agility));

    ui->clutchnessEdit->setText(QString::number(p->ment.clutchness));
    ui->riskinessEdit->setText(QString::number(p->ment.riskiness));
    ui->leadershipEdit->setText(QString::number(p->ment.leadership));
    ui->workRateEdit->setText(QString::number(p->ment.workRate));
    ui->toughnessEdit->setText(QString::number(p->ment.toughness));

    //check if player is a goalie or skater
    if(goalie* g = dynamic_cast<goalie*>(p)){
        ui->blockerEdit->setText(QString::number(g->goal.blocker));
        ui->gloveEdit->setText(QString::number(g->goal.glove));
        ui->gPositioningEdit->setText(QString::number(g->goal.gPositioning));
        ui->reflexesEdit->setText(QString::number(g->goal.reflexes));
        ui->specificStatsStackedWidget->setCurrentIndex(1);
    }
    else{
        skater* s = dynamic_cast<skater*>(p);

        ui->wristShotEdit->setText(QString::number(s->off.wristShot));
        ui->slapShotEdit->setText(QString::number(s->off.slapShot));
        ui->backhandEdit->setText(QString::number(s->off.backhand));
        ui->passingEdit->setText(QString::number(s->off.passing));
        ui->stickhandlingEdit->setText(QString::number(s->off.stickhandling));

        ui->faceoffsEdit->setText(QString::number(s->def.faceoffs));
        ui->dPositioningEdit->setText(QString::number(s->def.dPositioning));
        ui->checkingEdit->setText(QString::number(s->def.checking));

        ui->specificStatsStackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_applyStatsChangesButton_clicked()
{
    this->currentPlayer->phys.acceleration = ui->accelerationEdit->text().simplified().toInt();
    this->currentPlayer->phys.stamina = ui->staminaEdit->text().simplified().toInt();
    this->currentPlayer->phys.strength = ui->strengthEdit->text().simplified().toInt();
    this->currentPlayer->phys.speed = ui->speedEdit->text().simplified().toInt();
    this->currentPlayer->phys.agility = ui->agilityEdit->text().simplified().toInt();

    this->currentPlayer->ment.clutchness = ui->clutchnessEdit->text().simplified().toInt();
    this->currentPlayer->ment.riskiness = ui->riskinessEdit->text().simplified().toInt();
    this->currentPlayer->ment.leadership = ui->leadershipEdit->text().simplified().toInt();
    this->currentPlayer->ment.workRate = ui->workRateEdit->text().simplified().toInt();
    this->currentPlayer->ment.toughness = ui->toughnessEdit->text().simplified().toInt();


    //check if player is a goalie or skater
    if(goalie* g = dynamic_cast<goalie*>(this->currentPlayer)){
        g->goal.blocker = ui->blockerEdit->text().simplified().toInt();
        g->goal.glove = ui->gloveEdit->text().simplified().toInt();
        g->goal.gPositioning = ui->gPositioningEdit->text().simplified().toInt();
        g->goal.reflexes = ui->reflexesEdit->text().simplified().toInt();
    }
    else{
        skater* s = dynamic_cast<skater*>(this->currentPlayer);

        s->off.wristShot = ui->wristShotEdit->text().simplified().toInt();
        s->off.slapShot = ui->slapShotEdit->text().simplified().toInt();
        s->off.backhand = ui->backhandEdit->text().simplified().toInt();
        s->off.passing = ui->passingEdit->text().simplified().toInt();
        s->off.stickhandling = ui->stickhandlingEdit->text().simplified().toInt();

        s->def.faceoffs = ui->faceoffsEdit->text().simplified().toInt();
        s->def.dPositioning = ui->dPositioningEdit->text().simplified().toInt();
        s->def.checking = ui->checkingEdit->text().simplified().toInt();
    }
}

