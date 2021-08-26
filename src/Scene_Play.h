#pragma once
#include "Scene.h"
class Scene_Play : public Scene
{
protected:
	Entity earth;
	Entity shield;
	Entity satellite;

	size_t difficulty = 0;
	size_t timeAlive = 0;
	
	size_t satelliteCooldownMax = 0.5 * 60;
	size_t satelliteCooldown = satelliteCooldownMax;

	size_t timeUntilAsteroid = 120;
	size_t timeUntilGamma = 300;
	size_t timeUntilUFO = 160;

	std::vector<Sprite> _asteroidSprites;

	sf::Sound* currentMusic = nullptr;
	const size_t musicTrackCount = 2;
	size_t currentMusicTrack;

	bool renderDebug = false;
	bool alive = true;

	void handleCollisions();
	void handleMovement(Entity entity);
	void handleOrbit(Entity entity);
	void handleAnimations(Entity entity);
	void handleLifespan(Entity entity);
	void handleControls(Entity entity);

public:
	Scene_Play(GameEngine* engine);
	void tick();
	void render();
	void renderHealth();
	void init();
	void onKeyAction(std::string actionName, KeyAction action);

	void spawnAsteroid();
	void spawnGammaWarning();
	void spawnGamma(Vec2 dir);
	void spawnUFO();
};

