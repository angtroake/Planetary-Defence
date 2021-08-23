#pragma once
#include "base.h"
#include "EntityManager.h"

class GameEngine;

class Scene
{
protected:
	GameEngine* _engine = nullptr;
	sf::View _view;
	EntityManager _entityManager;

	std::map<int, std::string> _keyActionMap;
	virtual void tick() = 0;

public:
	Scene() {}
	Scene(GameEngine* engine);
	virtual void render() = 0;
	virtual void init() = 0;
	void simulate(const int frames);
	void renderEntity(const int& entity);
	
	virtual void onKeyAction(std::string actionName, KeyAction action) = 0;

	const std::map<int, std::string> & getKeyActionMap() const;
	void registerKeyAction(int key, std::string actionName);
};

