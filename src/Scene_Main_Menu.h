#pragma once
#include "Scene.h"
class Scene_Main_Menu : public Scene
{
protected:


public:
	Scene_Main_Menu(GameEngine* engine);
	void tick();
	void render();
	void init();

	void onKeyAction(std::string actionName, KeyAction action);
};

