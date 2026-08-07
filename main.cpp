#include <iostream>
#include <memory>
#include "player.hpp"
#include "skater.hpp"
#include "goalie.hpp"
#include "team.hpp"
#include "mainwindow.hpp"
#include <QApplication>
#include <dasmig/namegen.hpp>
#include <dasmig/random.hpp>


int main(int argc, char* argv[]) {
    //check if library works
    auto& gen = dasmig::ng::instance();
    std::wcout << gen.get_name(dasmig::gender::m, dasmig::culture::american);
    /*
	QApplication app(argc, argv);
    MainWindow window;
    window.show();
	return app.exec();
    */
}