#include "mainwindow.hpp"
#include "singleGameSimulation.hpp"
#include "ui_mainwindow.h"
#include "team.hpp"
#include <vector>
#include <random>
#include <dasmig/namegen.hpp>
#include <dasmig/random.hpp>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //for setupLines, spread columns over the given area
    ui->even4v4TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->goalieTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pk3v5TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pk4v5TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->forwards5v5TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->defense5v5TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ppTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->even4v4TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->goalieTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pk3v5TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->pk4v5TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->forwards5v5TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->defense5v5TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ppTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList positions = {"F", "D", "G"};
    ui->positionComboBox->addItems(positions);

    QStringList handednesses = {"L", "R"};
    ui->handednessComboBox->addItems(handednesses);

    currentRink = nullptr;
    currentSim = nullptr;
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
    if(awayTeamInSim == nullptr || homeTeamInSim == nullptr){
        QMessageBox::warning(this, "Input Error", "Teams for simulation haven't been selected.");
        return;
    }

    //set up sim
    rink *r = new rink(this);
    currentRink = r;
    ui->rinkView->setScene(r);
    ui->rinkView->scale(4, 4);
    singleGameSimulation* s = new singleGameSimulation(ui, awayTeamInSim, homeTeamInSim);
    currentSim = s;

    ui->awayLabel->setText("Away: " + awayTeamInSim->returnFullTeamName());
    ui->homeLabel->setText("Home: " + homeTeamInSim->returnFullTeamName());

    QObject::connect(ui->playSim, &QPushButton::clicked, this, [this]() {
        if (currentSim) currentSim->playSim();
    });

    QObject::connect(ui->pauseSim, &QPushButton::clicked, this, [this]() {
        if (currentSim) currentSim->pauseSim();
    });

    //set up timer
    currentSim->configureTimer();

    ui->stackedWidget->setCurrentIndex(2);

    //basically this should do everything
    currentSim->runSimulation();
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

    QListWidgetItem *newTeam = new QListWidgetItem();

    //makes team pointer retrievable later
    newTeam->setData(Qt::UserRole, QVariant::fromValue(t));

    ui->teamsListWidget->addItem(newTeam);

    //setting row format
    QWidget* rowWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(rowWidget);
    QLabel* teamName = new QLabel(t->returnFullTeamName());
    teamName->setFixedWidth(160);
    QPushButton* editTeam = new QPushButton("Edit");
    editTeam->setFixedWidth(60);
    QPushButton* deleteTeam = new QPushButton();
    deleteTeam->setIcon(deleteTeam->style()->standardIcon(QStyle::SP_TrashIcon));

    //select teams for simulation
    QCheckBox* awayTeam = new QCheckBox(rowWidget);
    QCheckBox* homeTeam = new QCheckBox(rowWidget);

    //makes it look nice
    layout->setContentsMargins(10, 2, 10, 2);
    layout->setSpacing(12);

    //add components
    layout->addWidget(teamName);
    layout->addStretch();
    layout->addWidget(awayTeam);
    awayTeam->setFixedWidth(40);
    layout->addStretch();
    layout->addWidget(homeTeam);
    homeTeam->setFixedWidth(40);
    layout->addStretch();
    layout->addWidget(editTeam);
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

    QObject::connect(awayTeam, &QCheckBox::checkStateChanged, [this, t, awayTeam](Qt::CheckState state) {
        bool isChecked = (state == Qt::Checked);
        if(isChecked){
            if(!awayTeamInSim && homeTeamInSim != t){
                awayTeamInSim = t;
            }
            else{
                QSignalBlocker blocker(awayTeam); //changing the checkState calls this function recursively, signal blocker prevents this
                awayTeam->setCheckState(Qt::Unchecked);
            }
        }
        else{
            awayTeamInSim = nullptr;
        }
    });

    QObject::connect(homeTeam, &QCheckBox::checkStateChanged, [this, t, homeTeam](Qt::CheckState state) {
        bool isChecked = (state == Qt::Checked);
        if(isChecked){
            if(!homeTeamInSim && awayTeamInSim != t){
                homeTeamInSim = t;
            }
            else{
                QSignalBlocker blocker(homeTeam); //changing the checkState calls this function recursively, signal blocker prevents this
                homeTeam->setCheckState(Qt::Unchecked);
            }
        }
        else{
            homeTeamInSim = nullptr;
        }
    });

    QObject::connect(editTeam, &QPushButton::clicked, [this, newTeam]() {
        if (!newTeam) return;

        // Retrieving team pointer
        this->currentTeam = newTeam->data(Qt::UserRole).value<team*>();

        ui->playersListWidget->clear();
        currentTeam->numDressed = 0;
        ui->numberOfPlayersLineEdit->setText(QString::number(currentTeam->roster.size()));
        // reload selected team's roster
        for (player* p : this->currentTeam->roster) {
            makePlayerListItem(p);
        }

        ui->teamNameLabel2->setText(this->currentTeam->returnFullTeamName());
        ui->playersDressedLabel->setText("Players Dressed: " + QString::number(currentTeam->numDressed) + "/20");
        ui->stackedWidget->setCurrentIndex(3);
    });
}

void MainWindow::on_backButton3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::makePlayerListItem(player* p){
    QListWidgetItem *newPlayer = new QListWidgetItem();
    //save player pointer
    newPlayer->setData(Qt::UserRole, QVariant::fromValue(p));

    ui->playersListWidget->addItem(newPlayer);

    //exclude used number
    currentTeam->numbersUsed[p->getNumber()] = true;
    //setting row format (same as team view basically)
    QWidget* playersRowWidget = new QWidget();
    QHBoxLayout* playersLayout = new QHBoxLayout(playersRowWidget);

    //ui design
    playersLayout->setContentsMargins(10, 2, 10, 2);
    playersLayout->setSpacing(12);


    QLabel* name = new QLabel(p->getFirstName() + " " + p->getLastName());
    name->setFixedWidth(160);

    QLabel* num = new QLabel(QString::number(p->getNumber()));
    num->setFixedWidth(40);
    num->setAlignment(Qt::AlignCenter);

    QCheckBox* dressed = new QCheckBox();
    dressed->setFixedWidth(40);
    if(p->isDressed){
        dressed->setCheckState(Qt::Checked);
        currentTeam->numDressed++;
    }

    QLabel* pos = new QLabel(p->getPositionAsString());
    pos->setFixedWidth(50);
    pos->setAlignment(Qt::AlignCenter);

    QLabel* handedness = new QLabel(p->getHandedness());
    handedness->setFixedWidth(60);
    handedness->setAlignment(Qt::AlignCenter);

    QLabel* country = new QLabel(p->getCountry());
    country->setFixedWidth(50);
    country->setAlignment(Qt::AlignCenter);

    QPushButton* editPlayer = new QPushButton("Edit");
    editPlayer->setFixedWidth(60);

    QPushButton* deletePlayer = new QPushButton();
    deletePlayer->setIcon(deletePlayer->style()->standardIcon(QStyle::SP_TrashIcon));
    deletePlayer->setFixedWidth(36);


    playersLayout->addWidget(name);
    playersLayout->addWidget(num);
    playersLayout->addWidget(dressed);
    playersLayout->addWidget(pos);
    playersLayout->addWidget(handedness);
    playersLayout->addWidget(country);
    playersLayout->addStretch();
    playersLayout->addWidget(editPlayer);
    playersLayout->addWidget(deletePlayer);

    newPlayer->setSizeHint(playersRowWidget->sizeHint());
    //attach layout to listItem
    ui->playersListWidget->setItemWidget(newPlayer, playersRowWidget);

    ui->numberOfPlayersLineEdit->setText(QString::number(currentTeam->roster.size()));

    //deletion mechanic
    //in the lambda: "this" is the list widget (row)
    QObject::connect(deletePlayer, &QPushButton::clicked, [this, newPlayer, p]() {
        int row = ui->playersListWidget->row(newPlayer);
        if (row != -1) { //if the row exists
            if(p->getIsDressed()){ //deleting a dressed player must also delete them from the lineup
                std::erase(currentTeam->lineup, p);
                currentTeam->numDressed--;
                p->isDressed = false; //we delete p anyway but whatever
                ui->playersDressedLabel->setText("Players Dressed: " + QString::number(currentTeam->numDressed) + "/20");
            }
            delete ui->playersListWidget->takeItem(row);
            std::erase(currentTeam->roster, p);
            delete p;
            ui->numberOfPlayersLineEdit->setText(QString::number(currentTeam->roster.size()));
        }});

    QObject::connect(dressed, &QCheckBox::checkStateChanged, [this, p, dressed](Qt::CheckState state) {
        bool isChecked = (state == Qt::Checked);
        if(isChecked){
            if(currentTeam->numDressed < 20){
                this->currentTeam->lineup.push_back(p);
                currentTeam->numDressed++;
                p->isDressed = true;
            }
            else{
                QSignalBlocker blocker(dressed); //changing the checkState calls this function recursively, signal blocker prevents this
                dressed->setCheckState(Qt::Unchecked);
            }
        }
        else{
            std::erase(currentTeam->lineup, p);
            currentTeam->numDressed--;
            p->isDressed = false;
        }
        ui->playersDressedLabel->setText("Players Dressed: " + QString::number(currentTeam->numDressed) + "/20");
    });

    QObject::connect(editPlayer, &QPushButton::clicked, [this, newPlayer, p]() {
        if (!newPlayer) return;

        // Retrieving player pointer
        this->currentPlayer = newPlayer->data(Qt::UserRole).value<player*>();

        ui->playerNameLabel->setText(this->currentPlayer->getFirstName() + " " + this->currentPlayer->getLastName());
        putPlayerAttributesToLineEdits(this->currentPlayer);
        ui->stackedWidget->setCurrentIndex(4);
        });

}

void MainWindow::on_backButton4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_addCustomPlayerButton_clicked()
{
    //check input validity
    if(ui->positionComboBox->currentText().isEmpty() || ui->handednessComboBox->currentText().isEmpty() || ui->numberLineEdit->text().simplified().isEmpty() || ui->firstNameLineEdit->text().simplified().isEmpty() || ui->lastNameLineEdit->text().simplified().isEmpty() ||  ui->countryLineEdit->text().simplified().isEmpty()){
        QMessageBox::warning(this, "Input Error", "Some fields are empty.");
        return;
    }
    if(ui->playersListWidget->findItems(ui->firstNameLineEdit->text().simplified() + " " + ui->lastNameLineEdit->text().simplified(), Qt::MatchContains).count() > 0){
        QMessageBox::warning(this, "Input Error", "This player already exists.");
        return;
    }
    if(currentTeam->numbersUsed[ui->numberLineEdit->text().simplified().toInt()]){
        QMessageBox::warning(this, "Input Error", "This number is already in use.");
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
        p = new goalie(ui->numberLineEdit->text().simplified().toInt(), ui->firstNameLineEdit->text().simplified(), ui->lastNameLineEdit->text().simplified(), handednessTemp,  ui->countryLineEdit->text().simplified());
    }
    else{
        if(ui->positionComboBox->currentText() == "F")
            positionTemp = 1;
        else
            positionTemp = 2;
        p = new skater(positionTemp, ui->numberLineEdit->text().simplified().toInt(), ui->firstNameLineEdit->text().simplified(), ui->lastNameLineEdit->text().simplified(), handednessTemp, ui->countryLineEdit->text().simplified());
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

void MainWindow::on_addRandomPlayerButton_clicked()
{
    int positionRand = getRandomNumber(1, 20); //12 F, 6 D, 2G ratio

    if(positionRand < 13){
        player* p = generateRandomPlayerOfGivenPosition(1);
        this->currentTeam->roster.push_back(p);
        makePlayerListItem(p);
    }
    else if(positionRand >= 13 && positionRand <= 18){
        player* p = generateRandomPlayerOfGivenPosition(2);
        this->currentTeam->roster.push_back(p);
        makePlayerListItem(p);
    }
    else{
        player* p = generateRandomPlayerOfGivenPosition(3);
        this->currentTeam->roster.push_back(p);
        makePlayerListItem(p);
    }
}

int MainWindow::getRandomNumber(int range1, int range2)
{
    // random
    std::random_device rd;

    // Mersenne Twister
    std::mt19937 gen(rd());

    // define range [inclusive, inclusive]
    std::uniform_int_distribution<int> distrib(range1, range2);

    // generate
    int randomNum = distrib(gen);

    return randomNum;
}
void MainWindow::on_fillRosterButton_clicked()
{
    int forwardCount = 0;
    int defensemanCount = 0;
    int goalieCount = 0;
    for(player* p : currentTeam->roster){
        if(p->getPositionAsString() == "F"){
            forwardCount++;
        }
        else if(p->getPositionAsString() == "D"){
            defensemanCount++;
        }
        else{
            goalieCount++;
        }

    }
    player* pl = nullptr;
    if(forwardCount < 14){
        for(int i = 0; i < 14 - forwardCount; i++){
            pl = generateRandomPlayerOfGivenPosition(1);
            this->currentTeam->roster.push_back(pl);
            makePlayerListItem(pl);
        }}
    if(defensemanCount < 7){
        for(int i = 0; i < 7 - defensemanCount; i++){
            pl = generateRandomPlayerOfGivenPosition(2);
            this->currentTeam->roster.push_back(pl);
            makePlayerListItem(pl);
        }}
    if(goalieCount < 2){
        for(int i = 0; i < 2 - goalieCount; i++){
            pl = generateRandomPlayerOfGivenPosition(3);
            this->currentTeam->roster.push_back(pl);
            makePlayerListItem(pl);
        }}
}

player* MainWindow::generateRandomPlayerOfGivenPosition(int position){
    auto& gen = dasmig::ng::instance();
    QString firstNamePicked;
    QString lastNamePicked;
    QString countryPicked;
    int numberRand = getRandomNumber(1, 98);
    while(currentTeam->numbersUsed[numberRand]){
        numberRand = getRandomNumber(1, 98); //keep generating a random number until you get one that isn't duplicate
    }
    int handednessRand = getRandomNumber(0, 1);

    int countryRand = getRandomNumber(1, 1000); // XX.X% format (country split based on NHL statistics)
    if(countryRand <= 422){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::canadian));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::canadian));
        countryPicked = "CA";
    } //canada
    else if(countryRand >= 423 && countryRand <= 700){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::american));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::american));
        countryPicked = "US";
    } //usa
    else if(countryRand >= 701 && countryRand <= 799){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::swedish));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::swedish));
        countryPicked = "SE";
    } //sweden
    else if(countryRand >= 800 && countryRand <= 865){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::russian));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::russian));
        countryPicked = "RU";
    } //russia
    else if(countryRand >= 866 && countryRand <= 915){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::finnish));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::finnish));
        countryPicked = "FI";
    } //finland
    else if(countryRand >= 916 && countryRand <= 943){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::czech));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::czech));
        countryPicked = "CZ";
    } //czech
    else if(countryRand >= 944 && countryRand <= 954){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::swiss));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::swiss));
        countryPicked = "CH";
    } //switzerland
    else if(countryRand >= 955 && countryRand <= 963){
        if(countryRand == 955){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::french));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::french));
            countryPicked = "FR";
        } //france
        else if(countryRand == 956){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::bulgarian));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::bulgarian));
            countryPicked = "BG";
        } //bulgaria
        else if(countryRand == 957){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::lithuanian));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::lithuanian));
            countryPicked = "LT";
        } //lithuania
        else if(countryRand == 958){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::british));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::british));
            countryPicked = "GB";
        }
        else if(countryRand == 959){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::polish));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::polish));
            countryPicked = "PL";
        } //poland
        else if(countryRand == 960){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::italian));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::italian));
            countryPicked = "IT";
        } //italy
        else if(countryRand == 961){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::belgian));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::belgian));
            countryPicked = "BE";
        } //belgium
        else if(countryRand == 962){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::chinese));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::chinese));
            countryPicked = "CN";
        } //china
        else{
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::brazilian));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::brazilian));
            countryPicked = "BR";
        } //brazil
    } //slovakia (no slovakian culture in the library, give to random unused countries)
    else if(countryRand >= 964 && countryRand <= 971){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::russian));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::russian));
        countryPicked = "BY";
    } //belarus (no belarussian name in library, just using russian names ig)
    else if(countryRand >= 972 && countryRand <= 979){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::german));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::german));
        countryPicked = "DE";
    } //germany
    else if(countryRand >= 980 && countryRand <= 986){
        if(countryRand == 980){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::kazakh));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::kazakh));
            countryPicked = "KZ";
        } //kazakhstan
        else if(countryRand == 981){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::nigerian));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::nigerian));
            countryPicked = "NG";
        } //nigeria
        else if(countryRand == 982){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::ukranian));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::ukranian));
            countryPicked = "UA";
        } //ukraine
        else if(countryRand == 983){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::korean));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::korean));
            countryPicked = "KR";
        } //south korea
        else if(countryRand == 984){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::dutch));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::dutch));
            countryPicked = "NL";
        } //netherlands
        else if(countryRand == 985){
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::japanese));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::japanese));
            countryPicked = "JP";
        } //japan
        else{
            firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::southafrican));
            lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::southafrican));
            countryPicked = "ZA";
        } //south africa
    } //latvia (no latvian culture in the library, give to random unused countries)
    else if(countryRand >= 987 && countryRand <= 993){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::danish));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::danish));
        countryPicked = "DK";
    } //denmark
    else if(countryRand >= 994 && countryRand <= 996){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::norwegian));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::norwegian));
        countryPicked = "NO";
    } //norway
    else if(countryRand >= 997 && countryRand <= 999){
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::austrian));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::austrian));
        countryPicked = "AT";
    } //austria
    else{
        firstNamePicked = QString::fromStdWString(gen.get_name(dasmig::gender::m, dasmig::culture::slovenian));
        lastNamePicked = QString::fromStdWString(gen.get_surname(dasmig::culture::slovenian));
        countryPicked = "SI";
    } //slovenia

    if(position == 3){
        return new goalie(numberRand, firstNamePicked, lastNamePicked, handednessRand, countryPicked);
    }
    return new skater(position, numberRand, firstNamePicked, lastNamePicked, handednessRand, countryPicked);
}

void MainWindow::on_setLinesButton_clicked()
{
    //reload dressed players
    for (player* p : currentTeam->lineup){
        QListWidgetItem *pl = new QListWidgetItem(p->getFirstName() + " " + p->getLastName() + " " + p->getPositionAsString());
        ui->playersToPickList->addItem(pl);
        //save player pointer
        pl->setData(Qt::UserRole, QVariant::fromValue(p));
    }
    refillTablesWithMappings();
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_backButton5_clicked()
{
    ui->playersToPickList->clear(); //reset list
    setMappingsFromTables();

    ui->forwards5v5TableWidget->clearContents(); //clear tables (but not headers)
    ui->defense5v5TableWidget->clearContents();
    ui->goalieTableWidget->clearContents();
    ui->even4v4TableWidget->clearContents();
    ui->pk3v5TableWidget->clearContents();
    ui->pk4v5TableWidget->clearContents();
    ui->ppTableWidget->clearContents();


    ui->stackedWidget->setCurrentIndex(3);

}


void MainWindow::on_playersToPickList_itemClicked(QListWidgetItem *item)
{
    //get player pointer
    player* p = item->data(Qt::UserRole).value<player*>();

    if(p->isHighlighted){
        //dehighlighting
        p->isHighlighted = false;
        item->setForeground(Qt::black);
    }
    else{
        for (QListWidgetItem* i : ui->playersToPickList->findItems("*", Qt::MatchWildcard)){
            player* pl = i->data(Qt::UserRole).value<player*>();
            pl->isHighlighted = false; //only one player highlighted at a time
            i->setForeground(Qt::black);
        }
    //set p to highlighted
    p->isHighlighted = true;

    //make text red to show it's highlighted
    item->setForeground(Qt::red);
    }
}

//Start of functions to place players into lines
player* MainWindow::findHighlightedPlayer(){
    for(player* p : currentTeam->lineup){
        if(p->isHighlighted){ return p; }
    }
    return nullptr;
}

QTableWidgetItem* MainWindow::returnItemToPlace(player* p){
    if(!p){return nullptr;} //if no player is highlighted, exit
    QTableWidgetItem* item = new QTableWidgetItem(p->getLastName());
    //save player pointer
    item->setData(Qt::UserRole, QVariant::fromValue(p));
    return item;
}

void MainWindow::on_forwards5v5TableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem* item = returnItemToPlace(findHighlightedPlayer());
    if(!item){return;}
    ui->forwards5v5TableWidget->setItem(row, column, item);
}

void MainWindow::on_defense5v5TableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem* item = returnItemToPlace(findHighlightedPlayer());
    if(!item){return;}
    ui->defense5v5TableWidget->setItem(row, column, item);
}

void MainWindow::on_goalieTableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem* item = returnItemToPlace(findHighlightedPlayer());
    if(!item){return;}
    ui->goalieTableWidget->setItem(row, column, item);
}

void MainWindow::on_pk3v5TableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem* item = returnItemToPlace(findHighlightedPlayer());
    if(!item){return;}
    ui->pk3v5TableWidget->setItem(row, column, item);
}

void MainWindow::on_pk4v5TableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem* item = returnItemToPlace(findHighlightedPlayer());
    if(!item){return;}
    ui->pk4v5TableWidget->setItem(row, column, item);
}

void MainWindow::on_even4v4TableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem* item = returnItemToPlace(findHighlightedPlayer());
    if(!item){return;}
    ui->even4v4TableWidget->setItem(row, column, item);
}

void MainWindow::on_ppTableWidget_cellClicked(int row, int column)
{
    QTableWidgetItem* item = returnItemToPlace(findHighlightedPlayer());
    if(!item){return;}
    ui->ppTableWidget->setItem(row, column, item);
}
//End of functions to place players into lines

void MainWindow::setMappingsFromTables(){

    //5v5forwards
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            QTableWidgetItem* item = ui->forwards5v5TableWidget->item(i, j);
            if(item && item->data(Qt::UserRole).isValid()){
                player* p = ui->forwards5v5TableWidget->item(i, j)->data(Qt::UserRole).value<player*>();
                if(p){
                    currentTeam->lineupMap[forward5v5Matrix[i][j]] = p; //create mapping
                }
            }
        }
    }

    //5v5defensemen
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            QTableWidgetItem* item = ui->defense5v5TableWidget->item(i, j);
            if(item && item->data(Qt::UserRole).isValid()){
                player* p = ui->defense5v5TableWidget->item(i, j)->data(Qt::UserRole).value<player*>();
                if(p){
                    currentTeam->lineupMap[defense5v5Matrix[i][j]] = p; //create mapping
                }
            }
        }
    }

    //pp
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 5; j++){
            QTableWidgetItem* item = ui->ppTableWidget->item(i, j);
            if(item && item->data(Qt::UserRole).isValid()){
                player* p = ui->ppTableWidget->item(i, j)->data(Qt::UserRole).value<player*>();
                if(p){
                    currentTeam->lineupMap[ppMatrix[i][j]] = p; //create mapping
                }
            }
        }
    }

    //pk3v5
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 3; j++){
            QTableWidgetItem* item = ui->pk3v5TableWidget->item(i, j);
            if(item && item->data(Qt::UserRole).isValid()){
                player* p = ui->pk3v5TableWidget->item(i, j)->data(Qt::UserRole).value<player*>();
                if(p){
                    currentTeam->lineupMap[pk3v5Matrix[i][j]] = p; //create mapping
                }
            }
        }
    }

    //pk4v5
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 4; j++){
            QTableWidgetItem* item = ui->pk4v5TableWidget->item(i, j);
            if(item && item->data(Qt::UserRole).isValid()){
                player* p = ui->pk4v5TableWidget->item(i, j)->data(Qt::UserRole).value<player*>();
                if(p){
                    currentTeam->lineupMap[pk4v5Matrix[i][j]] = p; //create mapping
                }
            }
        }
    }

    //even4v4
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 4; j++){
            QTableWidgetItem* item = ui->even4v4TableWidget->item(i, j);
            if(item && item->data(Qt::UserRole).isValid()){
                player* p = ui->even4v4TableWidget->item(i, j)->data(Qt::UserRole).value<player*>();
                if(p){
                    currentTeam->lineupMap[even4v4Matrix[i][j]] = p; //create mapping
                }
            }
        }
    }

    //goalies
    if(ui->goalieTableWidget->item(0, 0) && ui->goalieTableWidget->item(0, 0)->data(Qt::UserRole).isValid()){
        player* p = ui->goalieTableWidget->item(0, 0)->data(Qt::UserRole).value<player*>();\
        if(p){currentTeam->lineupMap[roles::G1] = ui->goalieTableWidget->item(0, 0)->data(Qt::UserRole).value<player*>();}
    }
    if(ui->goalieTableWidget->item(0, 1) && ui->goalieTableWidget->item(0, 1)->data(Qt::UserRole).isValid()){
        player* p = ui->goalieTableWidget->item(0, 1)->data(Qt::UserRole).value<player*>();
        if(p){currentTeam->lineupMap[roles::G2] = ui->goalieTableWidget->item(0, 1)->data(Qt::UserRole).value<player*>();}
    }
}

void MainWindow::refillTablesWithMappings(){

    //5v5forwards
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            player* p = currentTeam->lineupMap[forward5v5Matrix[i][j]];
            if(p){
                QTableWidgetItem* item = returnItemToPlace(p);
                ui->forwards5v5TableWidget->setItem(i, j, item);
            }
        }
    }

    //5v5defensemen
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            player* p = currentTeam->lineupMap[defense5v5Matrix[i][j]];
            if(p){
                QTableWidgetItem* item = returnItemToPlace(p);
                ui->defense5v5TableWidget->setItem(i, j, item);
            }
        }
    }

    //pp
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 5; j++){
            player* p = currentTeam->lineupMap[ppMatrix[i][j]];
            if(p){
                QTableWidgetItem* item = returnItemToPlace(p);
                ui->ppTableWidget->setItem(i, j, item);
            }
        }
    }

    //pk3v5
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 3; j++){
            player* p = currentTeam->lineupMap[pk3v5Matrix[i][j]];
            if(p){
                QTableWidgetItem* item = returnItemToPlace(p);
                ui->pk3v5TableWidget->setItem(i, j, item);
            }
        }
    }

    //pk4v5
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 4; j++){
            player* p = currentTeam->lineupMap[pk4v5Matrix[i][j]];
            if(p){
                QTableWidgetItem* item = returnItemToPlace(p);
                ui->pk4v5TableWidget->setItem(i, j, item);
            }
        }
    }

    //even4v4
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 4; j++){
            player* p = currentTeam->lineupMap[even4v4Matrix[i][j]];
            if(p){
                QTableWidgetItem* item = returnItemToPlace(p);
                ui->even4v4TableWidget->setItem(i, j, item);
            }
        }
    }

    //goalies
    if(currentTeam->lineupMap[roles::G1]){
        player* p = currentTeam->lineupMap[roles::G1];
        QTableWidgetItem* item = returnItemToPlace(p);
        ui->goalieTableWidget->setItem(0, 0, item);
    }
    if(currentTeam->lineupMap[roles::G2]){
        player* p = currentTeam->lineupMap[roles::G2];
        QTableWidgetItem *item = returnItemToPlace(p);
        ui->goalieTableWidget->setItem(0, 1, item);
    }
}