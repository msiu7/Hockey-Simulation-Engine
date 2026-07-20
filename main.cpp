#include <iostream>
#include <memory>
#include "player.hpp"
#include "skater.hpp"
#include "goalie.hpp"
#include "team.hpp"
#include <QApplication>
#include <QMainWindow>
int main(int argc, char* argv[]) {

	/*
	//Creating Some Players
	std::unique_ptr<player> arr[6];
	for (int i = 0; i < 5; i++) {
		arr[i] = std::make_unique<skater>();
		std::cout << *arr[i] << std::endl << std::endl;
	}
	arr[5] = std::make_unique<goalie>();
	std::cout << *(arr[5]) << std::endl << std::endl;

	//Creating A Team and displaying its roster
	team a = team();
	a.fillRoster();
	std::cout << a;
	*/
	QApplication app(argc, argv);
	QMainWindow window;
	window.show();

	return app.exec();
}