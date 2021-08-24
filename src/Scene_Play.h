#pragma once
#include "Scene.h"
class Scene_Play : public Scene
{
protected:
	Entity earth;
	size_t difficulty = 0;
	size_t timeAlive = 0;

	size_t timeUntilAsteroid = 120;

	std::vector<Sprite> _asteroidSprites;

public:
	Scene_Play(GameEngine* engine);
	void tick();
	void render();
	void init();
	void onKeyAction(std::string actionName, KeyAction action);

	void spawnAsteroid();
};

