#pragma once
#include "base.h"

class GameEngine;

class Scene
{
protected:
	GameEngine* _engine = nullptr;
	sf::View _view;

	std::map<int, std::string> _keyActionMap;

public:
	Scene() {}
	Scene(GameEngine* engine);
	virtual void tick() = 0;
	virtual void render() = 0;
	virtual void init() = 0;
	
	virtual void onKeyAction(std::string actionName, KeyAction action) = 0;

	const std::map<int, std::string> & getKeyActionMap() const;
	void registerKeyAction(int key, std::string actionName);
};

