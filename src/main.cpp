#include "engine.h"
#include <iostream>
#include "EntityManager.h"

int main() {
	std:srand(std::time(NULL));

	GameEngine gameEngine;
	gameEngine.run();

	gameEngine.getAssets().destroy();
	gameEngine.getWindow().close();

	return 0;
}