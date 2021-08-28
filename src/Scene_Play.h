#pragma once
#include "Scene.h"

enum BossType {
	MOTHERSHIP
};

class Scene_Play : public Scene
{
protected:
	Entity earth;
	Entity shield;
	Entity satellite;

	size_t difficulty = 0;
	size_t timeAlive = 0;
	
	size_t satelliteCooldownMax = 0.25 * 60;
	size_t satelliteCooldown = satelliteCooldownMax;

	size_t timeUntilAsteroid = randAsteroidTime();
	size_t timeUntilGamma = randGammaTime();
	size_t timeUntilUFO = randUFOTime();
	size_t timeUntilBoss = randBossTime();

	std::vector<Sprite> _asteroidSprites;

	sf::Sound* currentMusic = nullptr;
	const size_t musicTrackCount = 2;
	size_t currentMusicTrack;
	float musicFade = 1.0f;

	bool renderDebug = false;
	bool alive = true;

	
	bool isBoss = false;
	bool isBossWarning = false;
	Entity boss;
	BossType bossType;

	void handleCollisions();
	void handleMovement(Entity entity);
	void handleOrbit(Entity entity);
	void handleAnimations(Entity entity);
	void handleLifespan(Entity entity);
	void handleControls(Entity entity);
	void handleRope(Entity entity);
	void handleBoss();

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
	void spawnUFO2();
	void spawnBoss();
	void spawnBossWarning();
	void spawnHealth();

	size_t randUFOTime();
	size_t randAsteroidTime();
	size_t randGammaTime();
	size_t randBossTime();
};