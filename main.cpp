#include <iostream>
#include <memory>
#include "player.hpp"
#include "skater.hpp"
#include "goalie.hpp"
#include "team.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    //fix how the puck works with qml.
    qRegisterMetaType<puck*>("puck*");

	QApplication app(argc, argv);
    MainWindow window;
    //always start at the start screen
    window.generalBackButtonAction();

    window.show();
	return app.exec();
}