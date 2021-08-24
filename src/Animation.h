#pragma once
#include "base.h"

class GameEngine;
class EntityManager;

class Animation
{
protected:
	GameEngine* _engine;
	bool _isFinished = false;
	void finish();
	virtual void animate(const Entity& entity, EntityManager& entityManager) = 0;
	virtual void setup(const Entity& entity, EntityManager& entityManager) = 0;
	//virtual const std::vector<std::string> getSteps() const = 0;
public:
	Animation() {}
	Animation(GameEngine* gameEngine) : _engine(gameEngine) {}
	bool isFinished() const;
	void animateStep(const Entity& entity, EntityManager& entityManager);
	void init(const Entity& entity, EntityManager& entityManager);
};

class AnimationDrop : public Animation
{
protected:
	size_t step = 0;
	float targetY;

	Vec2 resetVelocity;

	void animate(const Entity& entity, EntityManager& entityManager) override;
	void setup(const Entity& entity, EntityManager& entityManager) override;
public:
	AnimationDrop(GameEngine* gameEngine, float targetY) : targetY(targetY) { _engine = gameEngine; }
	AnimationDrop(GameEngine* gameEngine, float targetY, Vec2 resetVelocity) : targetY(targetY), resetVelocity(resetVelocity){ _engine = gameEngine; }
};


