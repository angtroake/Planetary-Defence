#pragma once
#include "Scene.h"
class Scene_Play : public Scene
{
protected:
	Entity earth;
	Entity shield;
	size_t difficulty = 0;
	size_t timeAlive = 0;

	size_t timeUntilAsteroid = 120;
	size_t timeUntilGamma = 300;

	std::vector<Sprite> _asteroidSprites;

	bool renderDebug = false;
	bool alive = true;

	void handleCollisions();
	void handleMovement(Entity entity);
	void handleOrbit(Entity entity);
	void handleAnimations(Entity entity);
	void handleLifespan(Entity entity);

public:
	Scene_Play(GameEngine* engine);
	void tick();
	void render();
	void renderHealth();
	void init();
	void onKeyAction(std::string actionName, KeyAction action);

	void spawnAsteroid();
	void spawnGammaWarning();
	void spawnGamma(float angle);
};

