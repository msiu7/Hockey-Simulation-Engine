#include <iostream>
#include "player.hpp"

int main() {
	player arr[5];
	for (int i = 0; i < 5; i++) {
		arr[i] = player();
		std::cout << arr[i] << std::endl << std::endl;
	}
}