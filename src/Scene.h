#pragma once
#include "base.h"

class GameEngine;

class Scene
{
protected:
	GameEngine* _engine = nullptr;
	sf::View _view;

public:
	Scene(GameEngine* engine);
	virtual void tick() = 0;
	virtual void render() = 0;
};

