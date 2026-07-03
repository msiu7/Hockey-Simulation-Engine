#include <iostream>
#include <memory>
#include "player.hpp"
#include "skater.hpp"
#include "goalie.hpp"

int main() {
	std::unique_ptr<player> arr[6];
	for (int i = 0; i < 5; i++) {
		arr[i] = std::make_unique<skater>();
		std::cout << *arr[i] << std::endl << std::endl;
	}
	arr[5] = std::make_unique<goalie>();
	std::cout << *(arr[5]) << std::endl << std::endl;
}