#pragma once
#include "Scene.h"
class Scene_Play : public Scene
{
protected:
	Entity earth;

public:
	Scene_Play(GameEngine* engine);
	void tick();
	void render();
	void init();
	void onKeyAction(std::string actionName, KeyAction action);
};

