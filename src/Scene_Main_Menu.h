#pragma once
#include "Scene.h"
class Scene_Main_Menu : public Scene
{
protected:
	const std::string menuButtons[2] = {"Play", "Exit"};
	const int8_t menuButtonCount = 2;
	int8_t selectedMenuButton = 0;

public:
	Scene_Main_Menu(GameEngine* engine);
	void tick();
	void render();
	void init();

	void onKeyAction(std::string actionName, KeyAction action);
};

