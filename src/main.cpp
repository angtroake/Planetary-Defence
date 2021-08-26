#include "engine.h"

int main() {
	std:srand(std::time(NULL));

	GameEngine gameEngine;
	gameEngine.run();
	return 0;
}