#pragma once
#include "base.h"
#include "EntityManager.h"

class AI
{
protected:
	virtual void tick(GameEngine* engine) = 0;
public:
	AI() {}
	void simulate(GameEngine* engine);
};


class AIUFO : public AI 
{
protected:
	void tick(GameEngine* engine);
	int timeToShoot;
	EntityManager* entityManager;
	Vec2* target;
	Vec2* position;
public:
	AIUFO(Vec2* position, Vec2* target, EntityManager* entityManager);
};

class AIMothership : public AI 
{
protected:
	void tick(GameEngine* engine);
	int timeToShoot;
	int timeToSpawn;
	int shootCooldown;
	bool isShooting = false;
	EntityManager* entityManager;
	Vec2* target;
	Entity mothership;
public:
	AIMothership(Entity mothership, Vec2* target, EntityManager* entityManager);
	int randTimeToShoot();
	int randTimeToSpawn();
};
