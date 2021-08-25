#pragma once
#include "Scene.h"
class Scene_Gameover : public Scene
{
protected:
	void tick();
public:
	Scene_Gameover(GameEngine* engine);
	void render();
	void init();
	void onKeyAction(std::string actionName, KeyAction action);
};

